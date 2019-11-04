#include <time.h>

class dgps
{
  // calculating sat ecef position at the moment of calculation, from satellite ephemeris
  void ephemeris2ecefpos(sat_pos *sp, gtime_t t)
  {
    // return ephemeris to ecef pos

    // Time from ephemeris reference epoch
    // double t_k = TimeFromEpoch(t, t_oe)
    double t_k = timediff(t, sp->eph->toe)

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

  // calculating ecef postion from latitude longitude and height
  void latlon2ecefpos(double lat, double lon, double height, double *x, double *y, double *z)
  {
    x,y,z = 0;
  }

  // calculates pseudrange basestation correction value,
  // does that by calculating distance between sat and basestation
  // which resembles the pseudo_range_basestation_correction value.
  // needs to be applied to pseudo range from rover
  void calc_bprc(sat_pos *sp)
  {
    // pseudo_range_observed - pseudo_range_basestation_correction
  }

  // applying prc on pseudo range
  // should only be applied to rover pseudo range, writes result to pseudo_range_corrected value in sat_pos struct
  void apply_bprc(sat_pos *sp)
  {

  }

  // applying standard(ionospheric, tropospheric, multipath etc.) correction
  // can be applied to both, basestation and rover pseudo ranges
  void apply_scorrections(sat_pos *sp)
  {

  }

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
};
