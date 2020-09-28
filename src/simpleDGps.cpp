#include <cmath>

static double calcTimeFromEpoch(double t, double t_ref) 
{
  t-= t_ref;
  if      (t> 302400) t -= 604800;
  else if (t<-302400) t += 604800;
  return t;
}

static double calcEccentricAnomaly(ephemeris *ephem, double t_k) 
{
  // Semi-major axis
  int A = ephem->sqrtA*ephem->sqrtA;

  // Computed mean motion (rad/sec)
  double n_0 = sqrt(ephem->MU/(ephem->A*ephem->A*ephem->A));

  // Corrected mean motion
  double n = n_0 + ephem->dn;

  // Mean anomaly
  double M_k = ephem->M_0 + n*t_k;

  // Solve Kepler's Equation for Eccentric Anomaly
  double E_k = M_k;
  for(;;) 
  {
      double temp = E_k;
      E_k = M_k + ephem->e*sin(E_k);
      if (fabs(E_k - temp) < 1e-10) break;
  }

  return E_k;
}

static latLonAltPos ecefToLatLonAlt(ecefPos ecef)
{
  latLonAltPos finalLatLonPos = { 0, 0, 0 };
  double zp, w2, w, r2, r, s2, c2, s, c, ss;
  double g, rg, rf, u, v, m, f, p, x, y, z; 
  double n, lat, lon, alt;

  x = ecef.x;
  y = ecef.y;
  z = ecef.z;

  zp = abs(z);
  w2 = x*x + y*y;
  w = sqrt(w2);
  
  r2 = w2 + z*z;
  r = sqrt(r2);
  finalLatLonPos.lon = atan2(y, x);

  s2 = z*z/r2;
  c2 = w2/r2;
  u = A2/r;
  v = A3 - A4/r;

  if(c2 > 0.3)
  {
    s = (zp/r)*(1.0 + c2*(A1 + u + s2*v)/r);
    finalLatLonPos.lat = asin(s);
    ss = s*s;
    c = sqrt(1.0 - ss);
  } else
  {
    c = (w/r)*(1.0 - s2*( A5 - u - c2*v )/r);
    finalLatLonPos.lat = acos(c);
    ss = 1.0 - c*c;
    s = sqrt(ss);
  }
  g = 1.0 - E2*ss;
  rg = A/sqrt(g);
  rf = A6*rg;
  u = w - rg*c;
  v = zp - rf*s;
  f = c*u + s*v;
  m = c*v - s*u;
  p = m/( rf/g + f );

  finalLatLonPos.lat = finalLatLonPos.lat + p;
  finalLatLonPos.alt = f + m*p/2.0;
  if(z < 0.0)
  {
      finalLatLonPos.lat *= -1.0;
  }
  return finalLatLonPos;
}

static ecefPos latLonAltToEcef(latLonAltPos latlonAlt)
{
  double zp, w2, w, r2, r, s2, c2, s, c, ss;
  double g, rg, rf, u, v, m, f, p, x, y, z; 
  double n, lat, lon, alt;

  ecefPos ecef = { 0, 0, 0 };

  lat = latlonAlt.lat;
  lon = latlonAlt.lon;
  alt = latlonAlt.alt;

  n = A/sqrt(1 - E2 * sin(lat)* sin(lat));

  ecef.x = (n + alt )* cos(lat)* cos(lon);
  ecef.y = (n + alt )* cos(lat)* sin(lon);
  ecef.z = (n*(1 - E2 ) + alt)* sin(lat);

  return ecef;
}

// This function converts decimal degrees to radians
static double deg2rad(double deg) 
{
  return (deg * M_PI / 180);
}

//  This function converts radians to decimal degrees
static double rad2deg(double rad) 
{
  return (rad * 180 / M_PI);
}

static double **dmatrix(int nrl, int nrh, int ncl, int nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	int i,nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;
	/* allocate pointers to rows */
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	m += NR_END;
	m -= nrl;
	/* allocate rows and set pointers to them */
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	m[nrl] += NR_END;
	m[nrl] -= ncl;
	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
	/* return pointer to array of pointers to rows */
	return m;
}

static double *dvector(int nl, int nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;
	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	return v-nl+NR_END;
}

static void free_dvector(double *v, int nl, int nh)
/* free a double vector allocated with dvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

static double pythag(double a, double b)
/* compute (a2 + b2)^1/2 without destructive underflow or overflow */
{
	double absa,absb;
	absa=fabs(a);
	absb=fabs(b);
	if (absa > absb) return absa*sqrt(1.0+(absb/absa)*(absb/absa));
	else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+(absa/absb)*(absa/absb)));
}

/******************************************************************************/
static void svdcmp(double a[][4], int m, int n, double w[], double **v)
/*******************************************************************************
Given a matrix a[1..m][1..n], this routine computes its singular value
decomposition, A = U.W.VT.  The matrix U replaces a on output.  The diagonal
matrix of singular values W is output as a vector w[1..n].  The matrix V (not
the transpose VT) is output as v[1..n][1..n].
*******************************************************************************/
{
	int flag,i,its,j,jj,k,l,nm;
	double anorm,c,f,g,h,s,scale,x,y,z,*rv1;

	rv1=dvector(1,n);
	g=scale=anorm=0.0; /* Householder reduction to bidiagonal form */
	for (i=1;i<=n;i++) {
		l=i+1;
		rv1[i]=scale*g;
		g=s=scale=0.0;
		if (i <= m) {
			for (k=i;k<=m;k++) scale += fabs(a[k][i]);
			if (scale) {
				for (k=i;k<=m;k++) {
					a[k][i] /= scale;
					s += a[k][i]*a[k][i];
				}
				f=a[i][i];
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][i]=f-g;
				for (j=l;j<=n;j++) {
					for (s=0.0,k=i;k<=m;k++) s += a[k][i]*a[k][j];
					f=s/h;
					for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
				}
				for (k=i;k<=m;k++) a[k][i] *= scale;
			}
		}
		w[i]=scale *g;
		g=s=scale=0.0;
		if (i <= m && i != n) {
			for (k=l;k<=n;k++) scale += fabs(a[i][k]);
			if (scale) {
				for (k=l;k<=n;k++) {
					a[i][k] /= scale;
					s += a[i][k]*a[i][k];
				}
				f=a[i][l];
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][l]=f-g;
				for (k=l;k<=n;k++) rv1[k]=a[i][k]/h;
				for (j=l;j<=m;j++) {
					for (s=0.0,k=l;k<=n;k++) s += a[j][k]*a[i][k];
					for (k=l;k<=n;k++) a[j][k] += s*rv1[k];
				}
				for (k=l;k<=n;k++) a[i][k] *= scale;
			}
		}
		anorm = DMAX(anorm,(fabs(w[i])+fabs(rv1[i])));
	}
	for (i=n;i>=1;i--) { /* Accumulation of right-hand transformations. */
		if (i < n) {
			if (g) {
				for (j=l;j<=n;j++) /* Double division to avoid possible underflow. */
					v[j][i]=(a[i][j]/a[i][l])/g;
				for (j=l;j<=n;j++) {
					for (s=0.0,k=l;k<=n;k++) s += a[i][k]*v[k][j];
					for (k=l;k<=n;k++) v[k][j] += s*v[k][i];
				}
			}
			for (j=l;j<=n;j++) v[i][j]=v[j][i]=0.0;
		}
		v[i][i]=1.0;
		g=rv1[i];
		l=i;
	}
	for (i=IMIN(m,n);i>=1;i--) { /* Accumulation of left-hand transformations. */
		l=i+1;
		g=w[i];
		for (j=l;j<=n;j++) a[i][j]=0.0;
		if (g) {
			g=1.0/g;
			for (j=l;j<=n;j++) {
				for (s=0.0,k=l;k<=m;k++) s += a[k][i]*a[k][j];
				f=(s/a[i][i])*g;
				for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
			}
			for (j=i;j<=m;j++) a[j][i] *= g;
		} else for (j=i;j<=m;j++) a[j][i]=0.0;
		++a[i][i];
	}
	for (k=n;k>=1;k--) { /* Diagonalization of the bidiagonal form. */
		for (its=1;its<=30;its++) {
			flag=1;
			for (l=k;l>=1;l--) { /* Test for splitting. */
				nm=l-1; /* Note that rv1[1] is always zero. */
				if ((double)(fabs(rv1[l])+anorm) == anorm) {
					flag=0;
					break;
				}
				if ((double)(fabs(w[nm])+anorm) == anorm) break;
			}
			if (flag) {
				c=0.0; /* Cancellation of rv1[l], if l > 1. */
				s=1.0;
				for (i=l;i<=k;i++) {
					f=s*rv1[i];
					rv1[i]=c*rv1[i];
					if ((double)(fabs(f)+anorm) == anorm) break;
					g=w[i];
					h=pythag(f,g);
					w[i]=h;
					h=1.0/h;
					c=g*h;
					s = -f*h;
					for (j=1;j<=m;j++) {
						y=a[j][nm];
						z=a[j][i];
						a[j][nm]=y*c+z*s;
						a[j][i]=z*c-y*s;
					}
				}
			}
			z=w[k];
			if (l == k) { /* Convergence. */
				if (z < 0.0) { /* Singular value is made nonnegative. */
					w[k] = -z;
					for (j=1;j<=n;j++) v[j][k] = -v[j][k];
				}
				break;
			}
			if (its == 30) printf("no convergence in 30 svdcmp iterations");
			x=w[l]; /* Shift from bottom 2-by-2 minor. */
			nm=k-1;
			y=w[nm];
			g=rv1[nm];
			h=rv1[k];
			f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
			g=pythag(f,1.0);
			f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
			c=s=1.0; /* Next QR transformation: */
			for (j=l;j<=nm;j++) {
				i=j+1;
				g=rv1[i];
				y=w[i];
				h=s*g;
				g=c*g;
				z=pythag(f,h);
				rv1[j]=z;
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g = g*c-x*s;
				h=y*s;
				y *= c;
				for (jj=1;jj<=n;jj++) {
					x=v[jj][j];
					z=v[jj][i];
					v[jj][j]=x*c+z*s;
					v[jj][i]=z*c-x*s;
				}
				z=pythag(f,h);
				w[j]=z; /* Rotation can be arbitrary if z = 0. */
				if (z) {
					z=1.0/z;
					c=f*z;
					s=h*z;
				}
				f=c*g+s*y;
				x=c*y-s*g;
				for (jj=1;jj<=m;jj++) {
					y=a[jj][j];
					z=a[jj][i];
					a[jj][j]=y*c+z*s;
					a[jj][i]=z*c-y*s;
				}
			}
			rv1[l]=0.0;
			rv1[k]=f;
			w[k]=x;
		}
	}
	free_dvector(rv1,1,n);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
static double calcGeodeticDistance(double lat1d, double lon1d, double lat2d, double lon2d) 
{
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * EARTH_RADIUS_KM * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

static double calcSatToStationRange(ecefPos satPos, latLonAltPos baseStationPos) 
{
  ecefPos ecefBaseStationPoos = latLonAltToEcef(baseStationPos);
  return sqrt(pow(satPos.x-ecefBaseStationPoos.x, 2) + pow(satPos.y-ecefBaseStationPoos.y, 2) + pow(satPos.z-ecefBaseStationPoos.z, 2));
}

static ecefPos calcSatPos(ephemeris *ephem, double t) 
{ // Get satellite position at time t
  // Time from ephemeris reference epoch
  double t_k = calcTimeFromEpoch(t, ephem->t_oe);

  // Eccentric Anomaly
  double E_k = calcEccentricAnomaly(ephem, t_k);

  // True Anomaly
  double v_k = atan2(
    sqrt(1-ephem->e*ephem->e) * sin(E_k),
    cos(E_k) - ephem->e);

  // Argument of Latitude
  double AOL = v_k + ephem->omega;

  // Second Harmonic Perturbations
  double du_k = ephem->C_us*sin(2*AOL) + ephem->C_uc*cos(2*AOL);    // Argument of Latitude Correction
  double dr_k = ephem->C_rs*sin(2*AOL) + ephem->C_rc*cos(2*AOL);    // Radius Correction
  double di_k = ephem->C_is*sin(2*AOL) + ephem->C_ic*cos(2*AOL);    // Inclination Correction

  // Corrected Argument of Latitude; Radius & Inclination
  double u_k = AOL + du_k;
  double r_k = ephem->A*(1-ephem->e*cos(E_k)) + dr_k;
  double i_k = ephem->i_0 + di_k + ephem->IDOT*t_k;

  // Positions in orbital plane
  double x_kp = r_k*cos(u_k);
  double y_kp = r_k*sin(u_k);

  // Corrected longitude of ascending node
  double OMEGA_k = ephem->OMEGA_0 + (ephem->OMEGA_dot-ephem->OMEGA_E)*t_k - ephem->OMEGA_E*ephem->t_oe;

  ecefPos satPos = { 0, 0, 0 };

  // Earth-fixed coordinates
  satPos.x = x_kp*cos(OMEGA_k) - y_kp*cos(i_k)*sin(OMEGA_k);
  satPos.y = x_kp*sin(OMEGA_k) + y_kp*cos(i_k)*cos(OMEGA_k);
  satPos.z = y_kp*sin(i_k);

  return satPos;
}

static satRanges calcSatRangeCorrection(satLocation satPos, latLonAltPos baseStationPos, satRanges pseudoRanges) 
{
  satRanges trueRanges{};

  ecefPos pos;
  int id;
  double trueRange;

  std::map<int, ecefPos>::iterator it; 

  for (it = satPos.locations.begin(); it != satPos.locations.end(); it++)
  {
    pos = it->second;
    id = it->first;

    trueRange = calcSatToStationRange(pos, baseStationPos);

    trueRanges.ranges.insert(std::pair<int, double>(id, trueRange));
  }
  
  std::map<int, double>::iterator it_; 
  std::map<int, double>::iterator trueRangeMap, pseudoRangeMap;
  satRanges rangeCorrection;
  double correction = 0;
  for (it_ = trueRanges.ranges.begin(); it_ != trueRanges.ranges.end(); it_++)
  {
    pseudoRangeMap = pseudoRanges.ranges.find(it_->first);

    if (pseudoRangeMap != pseudoRanges.ranges.end()) 
    {
      correction = abs(it_->second - pseudoRangeMap->second);
      rangeCorrection.ranges.insert(std::pair<int, double>(it_->first, correction));
    } else
    {
      std::cout << "could not find pseudo range calculated true range to calc correction" << std::endl;
    }
  }
  return rangeCorrection;
}

static satRanges applyCorrectionOnPseudoRange(satRanges corrRanges, satRanges pseudoRanges) 
{
  std::map<int, double>::iterator it_; 
  std::map<int, double>::iterator pseudoRangeMap;
  satRanges rangeCorrection;
  double correctedRange = 0;
  for (it_ = corrRanges.ranges.begin(); it_ != corrRanges.ranges.end(); it_++)
  {
    // look up for pseudo range with same sat id
    pseudoRangeMap = pseudoRanges.ranges.find(it_->first);

    if (pseudoRangeMap != pseudoRanges.ranges.end()) 
    {
      correctedRange = it_->second;
      rangeCorrection.ranges.insert(std::pair<int, double>(it_->first, correctedRange));
    } else
    {
      std::cout << "could not find pseudo range for available correction" << std::endl;
    }
  }
  return rangeCorrection;
}

static ecefPos trillatPosFromRange(satLocation finalSatPos, satRanges finalSatRanges)
{
  std::map<int, ecefPos>::iterator it_; 
  std::map<int, double>::iterator finalSatRangesMap;

  double x, y, z;
  double Am, Bm, Cm, Dm;
  double range;
  int nSat = finalSatPos.locations.size();


  double matrixA[nSat][4];
  // manually initialize matrix
  memset(matrixA, 0,nSat*4*sizeof(double));

  int i = 0;
  for (it_ = finalSatPos.locations.begin(); it_ != finalSatPos.locations.end(); it_++)
  {
    // look up for pseudo range with same sat id
    finalSatRangesMap = finalSatRanges.ranges.find(it_->first);
    range = finalSatRangesMap->second;

    if (it_ != finalSatPos.locations.end()) 
    {
      x = it_->second.x;
      y = it_->second.y;
      z = it_->second.z;
      Am = -2*x;
      Bm = -2*y;
      Cm = -2*z;
      Dm = EARTH_RADIUS_KM*EARTH_RADIUS_KM + (pow(x,2)+pow(y,2)+pow(z,2)) - pow(range,2);
      // todo SVD
      matrixA[i][0] = Am;
      matrixA[i][1] = Bm;
      matrixA[i][2] = Cm;
      matrixA[i][3] = Dm;
      i++;
    } else
    {
      std::cout << "could not find sat pos for user pos trilateration" << std::endl;
    }
  }
  

  double **v;
  double w[] = {};
  svdcmp(matrixA, sizeof(matrixA)/sizeof(double), sizeof(matrixA[0])/sizeof(double), w, v);
  
  double ws = *v[3];
  // Resulting position in ECEF
  ws /= w[3];
  std::cout << (ws) << std::endl;
}