#include <time.h>

#include "gps_trilateration.cpp"

class dgps
{


  // calculating ecef postion from latitude longitude and height
  // https://github.com/swift-nav/libswiftnav/blob/master/src/coord_system.c
  // llh = lat lon height
  // ecef = x,y,z
  void latlon2ecefpos(double llh[2], double ecef[2])
  {
    double d = WGS84_E * sin(llh[0]);
    double N = WGS84_A / sqrt(1. - d * d);

    ecef[0] = (N + llh[2]) * cos(llh[0]) * cos(llh[1]);
    ecef[1] = (N + llh[2]) * cos(llh[0]) * sin(llh[1]);
    ecef[2] = ((1 - WGS84_E * WGS84_E) * N + llh[2]) * sin(llh[0]);
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
};
