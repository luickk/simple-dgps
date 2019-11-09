#include <time.h>
#include <algorithm>
#include <iostream>
#include <vector>

#include "gps_trilateration.cpp"

using std::vector;

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
    // pseudo_range_observed - true distance
    double sv_pos[2];
    sv_pos[0] = sp->pos[0]; // x, y, z
    sv_pos[1] = sp->pos[1]; // x, y, z
    sv_pos[2] = sp->pos[2]; // x, y, z

    return sp->pseudo_range_observed-calc_ecef_dist(sv_pos, bs_pos);
  }

  static bool sortBySatNo(const sat_pos &sp1, const sat_pos &sp2)
  {
    return sp1.satno > sp2.satno;
  }

  // applying prc on pseudo range
  // should only be applied to rover pseudo range, writes result to pseudo_range_corrected value in sat_pos struct
  // input params: sp = base station satellite position, rsp = rover satellite position
  void apply_bprc(vector<sat_pos> *bssp, vector<sat_pos> *rsp, double base_station_position[2])
  {
    // sort both struct arrays by satno
    std::sort(bssp->begin(), bssp->end(), &sortBySatNo);
    std::sort(rsp->begin(), rsp->end(), &sortBySatNo);

    for (int i=0; i<sp->size();i++)
    {
      // single sat, rover pos
      // single base station sat position (*bssp)[i];
      // single rover sat pos (*rsp)[i];

      if((*bssp)[i].satno == srp.satno)
      {
        // calc. prc value
        double bs_pos[2]; // x, y, z
        double bs_t_bias;

        double prc = calc_bprc(&(*bssp)[i], &base_station_position);

        // applying on base station satellites structs
        (*bssp)[i].pseudo_range_basestation_correction = prc;

        (*bssp)[i].pseudo_range_corrected = (*bssp)[i].pseudo_range_observed-(*bssp)[i].pseudo_range_basestation_correction;


        // applying on rover satellites structs
        (*rsp)[i].pseudo_range_basestation_correction = prc;

        (*rsp)[i].pseudo_range_corrected = (*rsp)[i].pseudo_range_observed-(*rsp)[i].pseudo_range_basestation_correction;


      }
    }
  }
};
