#include "gps_trilateration.cpp"

using std::vector;

// calculating ecef postion from latitude longitude and height
// https://github.com/swift-nav/libswiftnav/blob/master/src/coord_system.c
// llh = lat lon height
// ecef = x,y,z
void latloheightn2ecefpos(double llh[3], double ecef[3])
{
  double d = WGS84_E * sin(llh[0]);
  double N = WGS84_A / sqrt(1. - d * d);
  double x,y,z;
  ecef[0] = (N + llh[2]) * cos(llh[0]) * cos(llh[1]);
  ecef[1] = (N + llh[2]) * cos(llh[0]) * sin(llh[1]);
  ecef[2] = ((1 - WGS84_E * WGS84_E) * N + llh[2]) * sin(llh[0]);
}

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

      return sp->pseudo_range_observed-calc_ecef_dist(sv_pos, bs_pos);
    }

    /* variance by ura ephemeris -------------------------------------------------*/
    static double var_uraeph(int sys, int ura)
    {
        const double ura_value[]={
            2.4,3.4,4.85,6.85,9.65,13.65,24.0,48.0,96.0,192.0,384.0,768.0,1536.0,
            3072.0,6144.0
        };
        if (sys==SYS_GAL) { /* galileo sisa (ref [7] 5.1.11) */
            if (ura<= 49) return SQR(ura*0.01);
            if (ura<= 74) return SQR(0.5+(ura- 50)*0.02);
            if (ura<= 99) return SQR(1.0+(ura- 75)*0.04);
            if (ura<=125) return SQR(2.0+(ura-100)*0.16);
            return SQR(STD_GAL_NAPA);
        }
        else { /* gps ura (ref [1] 20.3.3.3.1.1) */
            return ura<0||15<ura?SQR(6144.0):SQR(ura_value[ura]);
        }
    }

    /* broadcast ephemeris to satellite position and clock bias --------------------
    * compute satellite position and clock bias with broadcast ephemeris (gps,
    * galileo, qzss)
    * args   : gtime_t time     I   time (gpst)
    *          eph_t *eph       I   broadcast ephemeris
    *          double *rs       O   satellite position (ecef) {x,y,z} (m)
    *          double *dts      O   satellite clock bias (s)
    *          double *var      O   satellite position and clock variance (m^2)
    * return : none
    * notes  : see ref [1],[7],[8]
    *          satellite clock includes relativity correction without code bias
    *          (tgd or bgd)
    *-----------------------------------------------------------------------------*/
    static void eph2pos(gtime_t time, eph_t *eph, sat_pos *sp)
    {
        double tk,M,E,Ek,sinE,cosE,u,r,i,O,sin2u,cos2u,x,y,sinO,cosO,cosi,mu,omge;
        double xg,yg,zg,sino,coso;
        int n,sys,prn;
        double dts, var;


        #ifdef LOG_DECODING_MSGS
        printf("eph2pos : time=%s sat=%2d\n", time_str(time,3),eph->sat);
        #endif

        if (eph->A<=0.0) {
            sp->pos[0]=sp->pos[1]=sp->pos[2]=dts=var=0.0;
            return;
        }
        tk=timediff(time,eph->toe);

        switch ((sys=satsys(eph->sat,&prn))) {
            case SYS_GAL: mu=MU_GAL; omge=OMGE_GAL; break;
            case SYS_CMP: mu=MU_CMP; omge=OMGE_CMP; break;
            default:      mu=MU_GPS; omge=OMGE;     break;
        }
        M=eph->M0+(sqrt(mu/(eph->A*eph->A*eph->A))+eph->deln)*tk;

        for (n=0,E=M,Ek=0.0;fabs(E-Ek)>RTOL_KEPLER&&n<MAX_ITER_KEPLER;n++) {
            Ek=E; E-=(E-eph->e*sin(E)-M)/(1.0-eph->e*cos(E));
        }
        if (n>=MAX_ITER_KEPLER) {

            #ifdef LOG_DECODING_MSGS
            printf("eph2pos: kepler iteration overflow sat=%2d\n",eph->sat);
            #endif
            return;
        }
        sinE=sin(E); cosE=cos(E);

        #ifdef LOG_DECODING_MSGS
        printf("kepler: sat=%2d e=%8.5f n=%2d del=%10.3e\n",eph->sat,eph->e,n,E-Ek);
        #endif

        u=atan2(sqrt(1.0-eph->e*eph->e)*sinE,cosE-eph->e)+eph->omg;
        r=eph->A*(1.0-eph->e*cosE);
        i=eph->i0+eph->idot*tk;
        sin2u=sin(2.0*u); cos2u=cos(2.0*u);
        u+=eph->cus*sin2u+eph->cuc*cos2u;
        r+=eph->crs*sin2u+eph->crc*cos2u;
        i+=eph->cis*sin2u+eph->cic*cos2u;
        x=r*cos(u); y=r*sin(u); cosi=cos(i);

        /* beidou geo satellite */
        if (sys==SYS_CMP&&(eph->flag==2||(eph->flag==0&&prn<=5))) {
            O=eph->OMG0+eph->OMGd*tk-omge*eph->toes;
            sinO=sin(O); cosO=cos(O);
            xg=x*cosO-y*cosi*sinO;
            yg=x*sinO+y*cosi*cosO;
            zg=y*sin(i);
            sino=sin(omge*tk); coso=cos(omge*tk);
            sp->pos[0]= xg*coso+yg*sino*COS_5+zg*sino*SIN_5;
            sp->pos[1]=-xg*sino+yg*coso*COS_5+zg*coso*SIN_5;
            sp->pos[2]=-yg*SIN_5+zg*COS_5;
        }
        else {
            O=eph->OMG0+(eph->OMGd-omge)*tk-omge*eph->toes;
            sinO=sin(O); cosO=cos(O);
            //std::cout << "x: " << x*cosO-y*cosi*sinO << "y: " << x*sinO+y*cosi*cosO << "z: " << y*sin(i) << std::endl;
            sp->pos[0]=x*cosO-y*cosi*sinO;
            sp->pos[1]=x*sinO+y*cosi*cosO;
            sp->pos[2]=y*sin(i);
        }
        tk=timediff(time,eph->toc);
        dts=eph->f0+eph->f1*tk+eph->f2*tk*tk;

        /* relativity correction */
        dts-=2.0*sqrt(mu*eph->A)*eph->e*sinE/SQR(CLIGHT);

        /* position and clock error variance */
        var=var_uraeph(sys,eph->sva);
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
