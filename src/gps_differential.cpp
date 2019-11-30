using std::vector;

class dgps
{
  private:
    static double calc_ecef_dist(double pos1[2], double pos2[2])
    {
      // formular for dist. calc.:
      // d=√(17−7)2+(6−4)2+(2−3)2
      return sqrt(pow(pos1[0]-pos2[0], 2) + pow(pos1[1]-pos2[1], 2) + pow(pos1[2]-pos2[2], 2));
    }

    // calculates pseudrange basestation correction value for a single satellite,
    // does that by calculating distance between sat and basestation
    // which resembles the pseudo_range_basestation_correction value.
    // needs to be applied to pseudo range from rover
    static double calc_bprc(sat_pos *sp, double bs_pos[3])
    {
      // pseudo_range_observed - true distance
      double sv_pos[2];
      sv_pos[0] = sp->pos[0]; // x, y, z
      sv_pos[1] = sp->pos[1]; // x, y, z
      sv_pos[2] = sp->pos[2]; // x, y, z

      double sv_bs_true_dist = calc_ecef_dist(sv_pos, bs_pos);

      // std::cout << "calc. sv - bs distance: " << sv_bs_true_dist << std::endl;

      return fabs(sp->pseudo_range_observed-sv_bs_true_dist);
    }

  public:
    // applying prc on pseudo range
    // should only be applied to rover pseudo range, writes result to pseudo_range_corrected value in sat_pos struct
    // input params: sp = base station satellite position, rsp = rover satellite position
    static void apply_prc(vector<sat_pos> *bssp, vector<sat_pos> *rsp, double base_station_position[3])
    {
      for (int i=0; i<bssp->size();i++)
      {
        // single sat, rover pos
        // single base station sat position (*bssp)[i];
        // single rover sat pos (*rsp)[i];

        if(std::find_if(rsp->begin(), rsp->end(), (*bssp)[i]) != rsp->end())
        {
          // calc. prc value
          double bs_pos[2]; // x, y, z
          double bs_t_bias;

          if ((*bssp)[i].eph.toe.time != 0 && (*bssp)[i].eph.A != 0 && (*bssp)[i].eph.e != 0 && (*bssp)[i].eph.deln != 0 && (*bssp)[i].eph.i0 != 0 && (*bssp)[i].eph.OMG0 != 0 && (*bssp)[i].eph.omg != 0 && (*bssp)[i].eph.M0 != 0 && (*bssp)[i].eph.OMGd != 0 && (*bssp)[i].eph.idot != 0 && (*bssp)[i].eph.crc != 0 && (*bssp)[i].eph.crs != 0 && (*bssp)[i].eph.cuc != 0 && (*bssp)[i].eph.cus != 0 && (*bssp)[i].eph.cic != 0 && (*bssp)[i].eph.cis != 0)
          {

            // calculating ecef coordinates of sat from ephemeris parameters
            gtime_t time_now;
            time_now.time = std::time(0);

            // ephemeris2ecefpos(&(*bssp)[i], time_now);

            eph2pos(time_now, &(*bssp)[i].eph,  &(*bssp)[i]);

            double prc = calc_bprc(&(*bssp)[i], base_station_position);

            // applying on base station satellites structs
            (*bssp)[i].pseudo_range_basestation_correction = prc;

            (*bssp)[i].pseudo_range_corrected = (*bssp)[i].pseudo_range_observed-(*bssp)[i].pseudo_range_basestation_correction;

            // applying on rover satellites structs
            (*rsp)[i].pseudo_range_basestation_correction = prc;

            (*rsp)[i].pseudo_range_corrected = (*rsp)[i].pseudo_range_observed-(*rsp)[i].pseudo_range_basestation_correction;

            // std::cout << "PRC is calculated \n" << std::endl;
          }

        } else
        {

        }
      }
    }
};
