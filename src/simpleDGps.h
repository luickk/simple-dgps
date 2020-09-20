#include <iostream>
#include <cmath>
#include <map>

using namespace std;

namespace simpleDGps 
{    
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
    
    // ephemeris calculations
    // by http://www.aholme.co.uk/GPS/SRC/2013/C++/ephemeris.cpp
    static double calcTimeFromEpoch(double t, double t_ref);
    static double calcEccentricAnomaly(ephemeris *ephem, double t_k);
    static double calcTimeFromEpoch(double t, double t_ref);
    static double calcEccentricAnomaly(ephemeris *ephem, double t_k);
    static ecefPos calcSatPos(ephemeris *ephem, double t);


    // latLonAlt distance calculations
    // by https://stackoverflow.com/questions/10198985/calculating-the-distance-between-2-latitudes-and-longitudes-that-are-saved-in-a
    static double deg2rad(double deg);
    static double rad2deg(double rad); 
    static double calcGeodeticDistance(double lat1d, double lon1d, double lat2d, double lon2d);


    // calculates non geodetic, 3 dim distance (straight line)
    static double calcSatToStationRange(ecefPos satPos, latLonAltPos baseStationPos);

    // calculates correction range from base station position, the sat pos and pseudo ranges of same fix loc
    static satRanges calcSatRangeCorrection(satLocation satPos, latLonAltPos baseStationPos, satRanges pseudoRanges);

    // applies corrections from calcSatRangeCorrection to given set of pseudo ranges. (to have range corrections applied, the same sat id's are required)
    static satRanges applyCorrectionOnPseudoRange(satRanges corrRanges, satRanges pseudoRanges);

    // calcs position from certain set of ranges (corrections need already to be applied)
    static latLonAltPos calcPosFromRange(satRanges finalRanges);

    // geodetic <-> ecef conversion
    static latLonAltPos ecefToLatLonAlt(ecefPos ecef);
    static ecefPos latLonAltToEcef(latLonAltPos latlonAlt);
};