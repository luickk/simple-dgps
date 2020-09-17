#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include <iostream>
#include <cmath>
#include <map>

using namespace std;

// conts for coord conversion transformation
const double  A = 6378137.0;              //WGS-84 semi-major axis
const double E2 = 6.6943799901377997e-3;  //WGS-84 first eccentricity squared
const double A1 = 4.2697672707157535e+4;  //A1 = a*e2
const double A2 = 1.8230912546075455e+9;  //A2 = a1*a1
const double A3 = 1.4291722289812413e+2;  //A3 = a1*e2/2
const double A4 = 4.5577281365188637e+9;  //A4 = 2.5*a2
const double A5 = 4.2840589930055659e+4;  //A5 = a1+a3
const double A6 = 9.9330562000986220e-1;  //A6 = 1-e2

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
    std::map<int, double> ranges;
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

static latLonAltPos ecefToLatLonAlt(ecefPos ecef);
static ecefPos latLonAltToEcef(latLonAltPos latlonAlt);

static satRanges calcTrueRange(ecefPos satPos, latLonAltPos baseStationPos);


#endif //PROJECT_MAIN_H