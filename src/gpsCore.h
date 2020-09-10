#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

void test();

struct ephemeris {
    // subframe 1
    int week;
    int t_gd;
    int IODC;
    int t_oc;
    int a_f[2];

    // subframe 2
    int IODE2;
    int C_rs;
    int dn;
    int M_0;
    int C_uc;
    int e;
    int C_us;
    int sqrtA;
    int t_oe;

    // subframe 3
    int C_ic;
    int OMEGA_0;
    int C_is;
    int i_0;
    int C_rc;
    int omega;
    int OMEGA_dot;
    int IODE3;
    int IDOT;
};

struct ecefPos {
    double x;
    double y;
    double z;
};

struct latLonAltPos {
    float lat;
    float lon;
    double alt;
};

// by http://www.aholme.co.uk/GPS/SRC/2013/C++/ephemeris.cpp
static double calcTimeFromEpoch(double t, double t_ref)
static int calcEccentricAnomaly(double t_K);
static ecefPos calcSatPos(ephemeris eph);

#endif //PROJECT_MAIN_H