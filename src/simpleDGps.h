#include "simpleDGps.cpp"

namespace simpleDGps 
{    
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


    //basic
    static double** multiplyMatrices(double matrixA[][posMTrillatAColumSize], double matrixB[][posMTrillatAColumSize], int matrixArows)
    static double** transpose2DimMatrix(double inputArr[][posMTrillatAColumSize]);
    
    // range position trillateration
    static double** calcInverse(int A[][posMTrillatAColumSize], int matrixArows);
    static double** calcAdjoint(double A[][posMTrillatAColumSize], int matrixArows);
    int clacDeterminant(double A[][posMTrillatAColumSize], int n);
    static double** leastSquareReg(double matrixA[][4]);
    
    static ecefPos trillatPosFromRange(satLocation finalSatPos, satRanges finalSatRanges);
};