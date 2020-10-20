#include <array>
#include <iostream>
#include <map>

using namespace std;

#define posMTrillatAColumSize 3

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

struct satLocation {
    // sat id, range
    std::map<int, ecefPos> locations;
};

struct latLonAltPos {
    float lat;
    float lon;
    double alt;
};


double** allocate2Ddouble(int row, int col);
void free2Dchar(char** final2D);

// ephemeris calculations
// by http://www.aholme.co.uk/GPS/SRC/2013/C++/ephemeris.cpp
double calcTimeFromEpoch(double t, double t_ref);
double calcEccentricAnomaly(ephemeris *ephem, double t_k);
double calcTimeFromEpoch(double t, double t_ref);
double calcEccentricAnomaly(ephemeris *ephem, double t_k);
ecefPos calcSatPos(ephemeris *ephem, double t);

// latLonAlt distance calculations
// by https://stackoverflow.com/questions/10198985/calculating-the-distance-between-2-latitudes-and-longitudes-that-are-saved-in-a
double deg2rad(double deg);
double rad2deg(double rad);
double calcGeodeticDistance(double lat1d, double lon1d, double lat2d, double lon2d);

// calculates non geodetic, 3 dim distance (straight line)
double calcSatToStationRange(ecefPos satPos, latLonAltPos baseStationPos);

// geodetic <-> ecef conversion
latLonAltPos ecefToLatLonAlt(ecefPos ecef);
ecefPos latLonAltToEcef(latLonAltPos latlonAlt);


//basic linear alg
double** multiplyMatrices(double **matrixA, double **matrixB, int matrixArows);
double** transpose2DimMatrix(double **inputArr, int matrixArows, int transpose2DimMatrix);
double** getCofactor(double **A, int p, int q, int n);
double clacDeterminant(double **A, int n);
double** calcInverse(double **A, int matrixArows);
double** calcAdjoint(double **A, int matrixArows);

// range position trillateration
double** leastSquareReg(double matrixA[][4]);

// returns position of given pseudo ranges via mult trilateration(solving via least square)
ecefPos trillatPosFromRange(satLocation finalSatPos, satRanges finalSatRanges);

// applies corrections from calcSatRangeCorrection to given set of pseudo ranges. (to have range corrections applied, the same sat id's are required)
satRanges applyCorrectionOnPseudoRange(satRanges corrRanges, satRanges pseudoRanges);
// calculates correction range from base station position, the sat pos and pseudo ranges of same fix loc
satRanges calcSatRangeCorrection(satLocation satPos, latLonAltPos baseStationPos, satRanges pseudoRanges);
