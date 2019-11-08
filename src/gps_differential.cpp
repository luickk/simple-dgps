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

  double calc_ecef_dist(double pos1[2], double pos2[2])
  {
    // formular for dist. calc.:
    // d=√(17−7)2+(6−4)2+(2−3)2

    return sqrt(pow(pos1[0]-pos2[0], 2) + pow(pos1[1]-pos2[1], 2) + pow(pos1[2]-pos2[2], 2));
  }

  // calculates pseudrange basestation correction value for a single satellite,
  // does that by calculating distance between sat and basestation
  // which resembles the pseudo_range_basestation_correction value.
  // needs to be applied to pseudo range from rover
  double calc_bprc(sat_pos *sp, double bs_pos[2])
  {
    // pseudo_range_observed - pseudo_range_basestation_correction
    double sv_pos[2];
    sv_pos[0] = sp->pos[0]; // x, y, z
    sv_pos[1] = sp->pos[1]; // x, y, z
    sv_pos[2] = sp->pos[2]; // x, y, z

    return calc_ecef_dist(sv_pos, bs_pos);
  }

  // applying prc on pseudo range
  // should only be applied to rover pseudo range, writes result to pseudo_range_corrected value in sat_pos struct
  void apply_bprc(sat_pos *sp[])
  {
    double bs_pos[2]; // x, y, z
    double bs_t_bias;
    int stat = solve_trilat(sp, &bs_pos[0], &bs_pos[1], &bs_pos[2], &bs_t_bias);

  }
};
