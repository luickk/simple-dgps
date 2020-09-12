#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

// ephemeris data from sat
struct ephemeris {
    double week;
    double t_gd;
    double IODC;
    double t_oc;
    double a_f[2];
    double A;
    double MU;
    double IODE2;
    double C_rs;
    double dn;
    double M_0;
    double C_uc;
    double e;
    double C_us;
    double sqrtA;
    double t_oe;
    double C_ic;
    double OMEGA_0;
    double OMEGA_E;
    double C_is;
    double i_0;
    double C_rc;
    double omega;
    double OMEGA_dot;
    double IODE3;
    double IDOT;
};

struct ecefPos {
    double x;
    double y;
    double z;
};

struct satRanges {
    // sat id, range
    map<int, double> ranges;
};

struct latLonAltPos {
    float lat;
    float lon;
    double alt;
};

// ephemeris calculations
// by http://www.aholme.co.uk/GPS/SRC/2013/C++/ephemeris.cpp
static double calcTimeFromEpoch(double t, double t_ref);
static double calcEccentricAnomaly(ephemeris *ephem, double t_k);
static ecefPos calcSatPos(ephemeris eph, double t);

#endif //PROJECT_MAIN_H