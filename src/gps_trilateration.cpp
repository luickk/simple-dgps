// by http://www.aholme.co.uk/GPS/SRC/2013/C++/solve.cpp

double A;
double M_0;
double sqrtA;
double e;
double dn;

double TimeFromEpoch(double t, double t_ref)
{
  t-= t_ref;
  if      (t> 302400) t -= 604800;
  else if (t<-302400) t += 604800;
  return t;
}

double EccentricAnomaly(double t_k)
{
  // Semi-major axis
  A = sqrtA*sqrtA;

  // Computed mean motion (rad/sec)
  double n_0 = sqrt(MU/(A*A*A));

  // Corrected mean motion
  double n = n_0 + dn;

  // Mean anomaly
  double M_k = M_0 + n*t_k;

  // Solve Kepler's Equation for Eccentric Anomaly
  double E_k = M_k;
  for(;;)
  {
      double temp = E_k;
      E_k = M_k + e*sin(E_k);
      if (fabs(E_k - temp) < 1e-10) break;
  }

  return E_k;
}

// calculating sat ecef position at the moment of calculation, from satellite ephemeris
// http://www.aholme.co.uk/GPS/SRC/2013/C++/ephemeris.cpp
void ephemeris2ecefpos(sat_pos *sp, gtime_t t)
{
  // return ephemeris to ecef pos

  // Time from ephemeris reference epoch
  // double t_k = TimeFromEpoch(t, t_oe)

  /* TODO: check for which ephemeris contains the data geph, peph etc. */
  gtime_t toe_gtime = sp->eph->toe; // eph reference epoch with week in gtime_t struct
  double t_oe = sp->eph->toe.time; // epg reference epoch in secs with weeks

  // IMPORTANT sqrtA could not be the same as eph param A
  sqrtA = sp->eph->A;
  // IMPORTANT
  e = sp->eph->e; // Eccentricity
  dn = sp->eph->deln; // delta n, mean anomalyat ref. epoch
  double i_0 = sp->eph->i0; // inclincation at ref. epoch
  double OMEGA_0 = sp->eph->OMG0; // long of ascending node at the beginning of the week
  double omega = sp->eph->omg; // rate of node'S right ascension
  M_0 = sp->eph->M0; // mean anomaly at ref. epoch
  double OMEGA_dot = sp->eph->OMGd; // rate of nodes right ascension
  double IDOT = sp->eph->idot; // rate if inclination angle

  double C_rc = sp->eph->crc; // orbital radius correction
  double C_rs = sp->eph->crs; // orbital radius correction
  double C_uc = sp->eph->cuc; // lat arg. correction
  double C_us = sp->eph->cus; // lat arg. correction
  double C_ic = sp->eph->cic; // inclination correction
  double C_is = sp->eph->cis; // inclination correction

  double t_k = timediff(t, toe_gtime);

  // Eccentric Anomaly
  double E_k = EccentricAnomaly(t_k);

  // True Anomaly
  double v_k = atan2(
      sqrt(1-e*e) * sin(E_k),
      cos(E_k) - e);

  // Argument of Latitude
  double AOL = v_k + omega;

  // Second Harmonic Perturbations
  double du_k = C_us*sin(2*AOL) + C_uc*cos(2*AOL);    // Argument of Latitude Correction
  double dr_k = C_rs*sin(2*AOL) + C_rc*cos(2*AOL);    // Radius Correction
  double di_k = C_is*sin(2*AOL) + C_ic*cos(2*AOL);    // Inclination Correction

  // Corrected Argument of Latitude; Radius & Inclination
  double u_k = AOL + du_k;
  double r_k = A*(1-e*cos(E_k)) + dr_k;
  double i_k = i_0 + di_k + IDOT*t_k;

  // Positions in orbital plane
  double x_kp = r_k*cos(u_k);
  double y_kp = r_k*sin(u_k);

  // Corrected longitude of ascending node
  double OMEGA_k = OMEGA_0 + (OMEGA_dot-OMEGA_E)*t_k - OMEGA_E*t_oe;

  // Earth-fixed coordinates
  double x = x_kp*cos(OMEGA_k) - y_kp*cos(i_k)*sin(OMEGA_k);
  double y = x_kp*sin(OMEGA_k) + y_kp*cos(i_k)*cos(OMEGA_k);
  double z = y_kp*sin(i_k);

  sp->pos[0] = x;
  sp->pos[1] = y;
  sp->pos[2] = x;
}

static void LatLonAlt(
    double x_n, double y_n, double z_n,
    double& lat, double& lon, double& alt) {

    const double a  = WGS84_A;
    const double e2 = WGS84_E2;

    const double p = sqrt(x_n*x_n + y_n*y_n);

    lon = 2.0 * atan2(y_n, x_n + p);
    lat = atan(z_n / (p * (1.0 - e2)));
    alt = 0.0;

    for (;;) {
        double tmp = alt;
        double N = a / sqrt(1.0 - e2*pow(sin(lat),2));
        alt = p/cos(lat) - N;
        lat = atan(z_n / (p * (1.0 - e2*N/(N + alt))));
        if (fabs(alt-tmp)<1e-3) break;
    }
}

double GetClockCorrection(double t, eph_t *eph) {

     // Time from ephemeris reference epoch
    double t_k = TimeFromEpoch(t, eph->toes);

    // Eccentric Anomaly
    double E_k = EccentricAnomaly(t_k);

    // Relativistic correction
    double t_R = F*eph->e*sqrtA*sin(E_k);

    // Time from clock correction epoch
    t = TimeFromEpoch(t, eph->toc.time);

    // 20.3.3.3.3.1 User Algorithm for SV Clock Correction
    // TODO: support group delay params not only for gps
    return eph->f0
         + eph->f1 * pow(t, 1)
         + eph->f2 * pow(t, 2) + t_R - eph->tgd[0];
}

// solves trilateration via. taylor series approximations without using determinant equation
// applies proportional clock correction for all sats
static int solve_trilat(std::vector <sat_pos> *sp, double *x_n, double *y_n, double *z_n, double *t_bias) {
    int i, j, r, c;

    int n_sats = sp->size();

    double t_tx[n_sats]; // Clock replicas in seconds since start of week

    double x_sv[n_sats],
           y_sv[n_sats],
           z_sv[n_sats];

    double t_pc;  // Uncorrected system time when clock replica snapshots taken
    double t_rx;    // Corrected GPS time

    double dPR[n_sats]; // Pseudo range error

    double jac[n_sats][4], ma[4][4], mb[4][4], mc[4][n_sats], md[4];

    double weight[n_sats];

    *x_n = *y_n = *z_n = *t_bias = t_pc = 0;

    for (i=0; i<n_sats; i++) {

        weight[i] = (*sp)[i].SNR;

        // Un-corrected time of transmission
        t_tx[i] = (*sp)[i].time_of_eph_observation;

        // Clock correction
        t_tx[i] -= GetClockCorrection(t_tx[i], (*sp)[i].eph);

        gtime_t t_tx_;
        t_tx_.time = t_tx[i];

        ephemeris2ecefpos(&(*sp)[i], t_tx_);

        x_sv[i] = (*sp)[i].pos[0]+i;
        y_sv[i] = (*sp)[i].pos[1]+i;
        z_sv[i] = (*sp)[i].pos[2]+i;

        t_pc += t_tx[i];
    }

    // Approximate starting value for receiver clock
    t_pc = t_pc/n_sats + 75e-3;

    // Iterate to user xyzt solution using Taylor Series expansion:
    for(j=0; j<MAX_ITER; j++) {
        // NextTask();

        t_rx = t_pc - *t_bias;

        for (i=0; i<n_sats; i++) {
            // Convert SV position to ECI coords (20.3.3.4.3.3.2)
            double theta = (t_tx[i] - t_rx) * OMEGA_E;

            double x_sv_eci = x_sv[i]*cos(theta) - y_sv[i]*sin(theta);
            double y_sv_eci = x_sv[i]*sin(theta) + y_sv[i]*cos(theta);
            double z_sv_eci = z_sv[i];

            // Geometric range (20.3.3.4.3.4)
            double gr = sqrt(pow(*x_n - x_sv_eci, 2) +
                             pow(*y_n - y_sv_eci, 2) +
                             pow(*z_n - z_sv_eci, 2));

            dPR[i] = C*(t_rx - t_tx[i]) - gr;

            jac[i][0] = (*x_n - x_sv_eci) / gr;
            jac[i][1] = (*y_n - y_sv_eci) / gr;
            jac[i][2] = (*z_n - z_sv_eci) / gr;
            jac[i][3] = C;
        }

        // ma = transpose(H) * W * H
        for (r=0; r<4; r++)
            for (c=0; c<4; c++) {
            ma[r][c] = 0;
            for (i=0; i<n_sats; i++) ma[r][c] += jac[i][r]*weight[i]*jac[i][c];
        }

        // mc = inverse(transpose(H)*W*H) * transpose(H)
        for (r=0; r<4; r++)
            for (c=0; c<n_sats; c++) {
            mc[r][c] = 0;
            for (i=0; i<4; i++) mc[r][c] += mb[r][i]*jac[c][i];
        }
        // md = inverse(transpose(H)*W*H) * transpose(H) * W * dPR
        for (r=0; r<4; r++) {
            md[r] = 0;
            for (i=0; i<n_sats; i++) md[r] += mc[r][i]*weight[i]*dPR[i];
        }

        double dx = md[0];
        double dy = md[1];
        double dz = md[2];
        double dt = md[3];

        double err_mag = sqrt(dx*dx + dy*dy + dz*dz);

        // printf("%14g%14g%14g%14g%14g\n", err_mag, t_bias, x_n, y_n, z_n);

        if (err_mag<1.0) break;

        *x_n    += dx;
        *y_n    += dy;
        *z_n    += dz;
        *t_bias += dt;
    }

    // UserStat(STAT_TIME, t_rx);
    return j;
}
