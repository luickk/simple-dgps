#include "simpleDGps.h"

#include <cmath>

// conts for coord conversion transformation
const double  A = 6378137.0;              //WGS-84 semi-major axis
const double EARTH_RADIUS_KM = 6378.8; // earth radius in km
const double E2 = 6.6943799901377997e-3;  //WGS-84 first eccentricity squared
const double A1 = 4.2697672707157535e+4;  //A1 = a*e2
const double A2 = 1.8230912546075455e+9;  //A2 = a1*a1
const double A3 = 1.4291722289812413e+2;  //A3 = a1*e2/2
const double A4 = 4.5577281365188637e+9;  //A4 = 2.5*a2
const double A5 = 4.2840589930055659e+4;  //A5 = a1+a3
const double A6 = 9.9330562000986220e-1;  //A6 = 1-e2

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


// Function to get cofactor of A[p][q] in temp[][]. n is current
// dimension of A[][]
static double** getCofactor(double **A, int p, int q, int n)
{
  double** temp = new double*[posMTrillatAColumSize];
  int i = 0, j = 0;

  // Looping for each element of the matrix
  for (int row = 0; row < n; row++)
  {
      for (int col = 0; col < n; col++)
      {
          //  Copying into temporary matrix only those element
          //  which are not in given row and column
          if (row != p && col != q)
          {
              temp[i][j++] = A[row][col];

              // Row is filled, so increase row index and
              // reset col index
              if (j == n - 1)
              {
                  j = 0;
                  i++;
              }
          }
      }
  }
  return temp;
}

/* Recursive function for finding determinant of matrix.
  n is current dimension of A[][]. */
static double clacDeterminant(double **A, int n)
{
  double D = 0; // Initialize result

  //  Base case : if matrix contains single element
  if (n == 1)
      return A[0][0];

  int sign = 1;  // To store sign multiplier

    // Iterate for each element of first row
  for (int f = 0; f < n; f++)
  {
      // Getting Cofactor of A[0][f]
      double **temp = getCofactor(A, 0, f, n);
      D += sign * A[0][f] * clacDeterminant(A, n - 1);

      // terms are to be added with alternate sign
      sign = -sign;
  }

  return D;
}

// Function to get adjoint of A[N][N] in adj[N][N].
static double** calcAdjoint(double **A, int matrixArows)
{
  double** adj = new double*[matrixArows];
  int sign = 0;

  for (int i=0; i<matrixArows; i++)
  {
      for (int j=0; j<posMTrillatAColumSize; j++)
      {
          // Get cofactor of A[i][j]
          double **temp = getCofactor(A, i, j, posMTrillatAColumSize);

          // sign of adj[j][i] positive if sum of row
          // and column indexes is even.
          sign = ((i+j)%2==0)? 1: -1;

          // Interchanging rows and columns to get the
          // transpose of the cofactor matrix
          adj[j][i] = (sign)*(clacDeterminant(A, posMTrillatAColumSize-1));
      }
  }
  return adj;
}

// by https://www.programiz.com/cpp-programming/examples/matrix-multiplication-function modified by L.K.
// method assumes that matrices have same dim sizes
static double** multiplyMatrices(double **matrixA, double **matrixB, int matrixArows)
{
  double** outputMatrix = new double*[posMTrillatAColumSize];
  int i, j, k;

  // Initializing elements of matrix mult to 0.
  for(i = 0; i < matrixArows; ++i)
  {
    for(j = 0; j < posMTrillatAColumSize; ++j)
    {
      outputMatrix[i][j] = 0;
    }
  }

  // Multiplying matrix matrixA and matrixB and storing in array mult.
  for(i = 0; i < matrixArows; ++i)
  {
    for(j = 0; j < posMTrillatAColumSize; ++j)
    {
      for(k=0; k<posMTrillatAColumSize; ++k)
      {
        outputMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }
  return outputMatrix;
}

// by https://www.programiz.com/cpp-programming/examples/matrix-multiplication-function modified by L.K.
// method assumes that matrices have same dim sizes
static double** multiplyMatricesND1D(double **matrixA, double **matrixB, int matrixArows)
{
  double** outputMatrix = new double*[posMTrillatAColumSize];
  int i, j, k;

  // Initializing elements of matrix mult to 0.
  for(i = 0; i < matrixArows; ++i)
  {
    for(j = 0; j < posMTrillatAColumSize; ++j)
    {
      outputMatrix[i][j] = 0;
    }
  }

  // Multiplying matrix matrixA and matrixB and storing in array mult.
  for(i = 0; i < matrixArows; ++i)
  {
    for(j = 0; j < posMTrillatAColumSize; ++j)
    {
      for(k=0; k<posMTrillatAColumSize; ++k)
      {
        outputMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }
  return outputMatrix;
}

static double** transpose2DimMatrix(double **inputArr, int matrixArows, int transpose2DimMatrix)
{
  double **outputArr = new double*[matrixArows];
  for (int i = 0; i < matrixArows; ++i)
  {
    for (int j = 0; j < transpose2DimMatrix; ++j)
    {
      outputArr[j][i]= inputArr[i][j];
    }
  }
  return outputArr;
}

// Function to calculate and store inverse, returns 0 if false
// matrix is singular by https://www.geeksforgeeks.org/adjoint-inverse-matrix/
static double** calcInverse(double **A, int matrixArows)
{
  double** inverse = new double*[posMTrillatAColumSize];
  // Find determinant of A[][]
  int det = clacDeterminant(A, posMTrillatAColumSize);
  if (det == 0)
  {
      cout << "Singular matrix, can't find its inverse";
      return 0;
  }

  // Find adjoint
  double **adj = calcAdjoint(A, matrixArows);

  // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
  for (int i=0; i<matrixArows; i++)
      for (int j=0; j<posMTrillatAColumSize; j++)
          inverse[i][j] = adj[i][j]/float(det);

  return inverse;
}

static double** leastSquareReg(double **matrixA, double **matrixB, int matrixArows, int matrixAcol)
{
  double **matrixATransposed = transpose2DimMatrix(matrixA, matrixArows, matrixAcol);
  double **matrixATransposedA = multiplyMatrices(matrixATransposed, matrixA, matrixArows);
  double **matrixATransposedAInverse = calcInverse(matrixATransposedA, matrixArows);
  double **matrixATransposedAAdd = multiplyMatrices(matrixATransposedAInverse, matrixATransposed, matrixArows);
  double **finalX = multiplyMatricesND1D(matrixATransposedAAdd, matrixB, matrixArows);

  return finalX;
}

ecefPos trillatPosFromRange(satLocation finalSatPos, satRanges finalSatRanges)
{
  std::map<int, ecefPos>::iterator it_;
  std::map<int, double>::iterator finalSatRangesMap;
  int matrixArows, matrixAcol = 0;
  double x, y, z;
  double Am, Bm, Cm, Dm;
  double range;
  int nSat = finalSatPos.locations.size();
  ecefPos finalPos = { 0.0, 0.0, 0.0 };
  double **matrixA = new double*[3];
  double **matrixB = new double*[1];

  matrixAcol = posMTrillatAColumSize;
  matrixArows = nSat;

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

      matrixA[i][0] = Am;
      matrixA[i][1] = Bm;
      matrixA[i][2] = Cm;
      matrixB[i][0] = Dm;
      i++;
    } else
    {
      std::cout << "could not find sat pos for user pos trilateration" << std::endl;
    }
  }

  // least square regression
  double **finalECEF = leastSquareReg(matrixA, matrixB, matrixArows, matrixAcol);

  finalPos.x = *finalECEF[0];
  finalPos.y = *finalECEF[1];
  finalPos.z = *finalECEF[2];

  return finalPos;
}
