#include <iostream>
using namespace std;
#include "gpsCore.h"

void test() {
  cout << "core GPS test!";
}

static double calcTimeFromEpoch(double t, double t_ref) {
    t-= t_ref;
    if      (t> 302400) t -= 604800;
    else if (t<-302400) t += 604800;
    return t;
}


static double calcEccentricAnomaly(ephemeris &ephem, double t_k) {

    // Semi-major axis
    int A = ephem->sqrtA*ephem->sqrtA;

    // Computed mean motion (rad/sec)
    double n_0 = sqrt(ephem->MU/(ephem->A*ephem->A*ephem->A));

    // Corrected mean motion
    double n = n_0 + ephem->dn;

    // Mean anomaly
    double M_k = M_0 + n*t_k;

    // Solve Kepler's Equation for Eccentric Anomaly
    double E_k = M_k;
    for(;;) {
        double temp = E_k;
        E_k = M_k + ephem->e*sin(E_k);
        if (fabs(E_k - temp) < 1e-10) break;
    }

    return E_k;
}

static ecefPos calcSatPos(ephemeris &ephem) { // Get satellite position at time t

     // Time from ephemeris reference epoch
    double t_k = TimeFromEpoch(t, t_oe);

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
    *x = x_kp*cos(OMEGA_k) - y_kp*cos(i_k)*sin(OMEGA_k);
    *y = x_kp*sin(OMEGA_k) + y_kp*cos(i_k)*cos(OMEGA_k);
    *z = y_kp*sin(i_k);
}