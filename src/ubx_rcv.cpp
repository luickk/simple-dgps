#include <iostream>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "sdgps_defines.h"

#include "data.h"
#include "nav_data_decoding.cpp"

/* class contains methods to decode raw ubx sfrbx data msgs */

class M8T
{
  public:
    static int decode_UBX_RXM_SFRBX_msg(raw_t* raw, std::vector<sat_pos> *satellites_array, unsigned char data)
    {
      /* synchronize frame */
      if (raw->nbyte==0) {
        if (!sync_ubx(raw->buff,data)) return 0;
        raw->nbyte=2;
        return 0;
      }
      raw->buff[raw->nbyte++]=data;

      if (raw->nbyte==6) {
        if ((raw->len=U2(raw->buff+4)+8)>MAXRAWLEN) {
            #ifdef LOG_DECODING_MSGS
            printf("ubx length error: len=%d\n",raw->len);
            #endif
            raw->nbyte=0;
            return -1;
        }
      }
      if (raw->nbyte<6||raw->nbyte<raw->len) return 0;
      raw->nbyte=0;

      int type=(U1(raw->buff+2)<<8)+U1(raw->buff+3);

      #ifdef LOG_DECODING_MSGS
      printf("decode_ubx: type=%04x len=%d\n",type,raw->len);
      #endif

      /* checksum */
      if (!checksum(raw->buff,raw->len)) {
          #ifdef LOG_DECODING_MSGS
          printf("ubx checksum error: type=%04x len=%d\n",type,raw->len);
          #endif
          return -1;
      }

      decode_rxmsfrbx(raw, satellites_array);

      if (raw->outtype) {
        #ifdef LOG_DECODING_MSGS
        sprintf(raw->msgtype,"UBX 0x%02X 0x%02X (%4d)",type>>8,type&0xF, raw->len);
        #endif
      }
      return 0;
    }

  static void print_sat_pos_array(std::vector<sat_pos> *satellites_array)
  {
    printf("------------------%d------------------- \n", satellites_array->size());
    for (int i=0; i<satellites_array->size();i++)
    {
      eph_t sp_eph = (*(*satellites_array)[i].eph);
      printf("-------");
      printf("satno: %d \n time_of_ob: %d \n pseudo_range_observed: %d \n pseudo_range_basestation_correction: %d \n",
      (*satellites_array)[i].satno,(*satellites_array)[i].time_of_eph_observation,(*satellites_array)[i].pseudo_range_observed,(*satellites_array)[i].pseudo_range_basestation_correction);
      printf("iode: %d \n iodc: %d \n sva: %d \n svh: %d \n week: %d \n code: %d \n flag: %d \n toe: %d \n toc: %d \n ttr: %d \n A: %d \n e: %d \n i0: %d \n OMG0: %d \n omg: %d \n M0: %d \n deln: %d \n OMGd: %d \n idot: crc: %d \n crs: %d \n cuc: %d \n cus: %d \n cic: %d \n cis: %d \n toes: %d \n fit: %d \n f0: %d \n f1: %d \n f2: %d \n tgd: %d \n Adot: %d \n ndot: %d \n ",
      sp_eph.iode,sp_eph.iodc,sp_eph.sva,sp_eph.svh,sp_eph.week,sp_eph.code,sp_eph.flag,sp_eph.toe,sp_eph.toc,sp_eph.ttr,sp_eph.A,sp_eph.e,sp_eph.i0,sp_eph.OMG0,sp_eph.omg,sp_eph.M0,sp_eph.deln,sp_eph.OMGd,sp_eph.idot,sp_eph.crc,sp_eph.crs,sp_eph.cuc,sp_eph.cus,sp_eph.cic,sp_eph.cis,sp_eph.toes,sp_eph.fit,sp_eph.f0,sp_eph.f1,sp_eph.f2,sp_eph.tgd,sp_eph.Adot,sp_eph.ndot);
    }
  }
  private:
    /* ubx gnssid to system (ref [2] 25) -----------------------------------------*/
    static int ubx_sys(int gnssid)
    {
        switch (gnssid) {
            case 0: return SYS_GPS;
            case 1: return SYS_SBS;
            case 2: return SYS_GAL;
            case 3: return SYS_CMP;
            case 5: return SYS_QZS;
            case 6: return SYS_GLO;
        }
        return 0;
    }

    /* sync code -----------------------------------------------------------------*/
    static int sync_ubx(unsigned char *buff, unsigned char data)
    {
        buff[0]=buff[1]; buff[1]=data;
        return buff[0]==UBXSYNC1&&buff[1]==UBXSYNC2;
    }

    /* checksum ------------------------------------------------------------------*/
    static int checksum(unsigned char *buff, int len)
    {
        unsigned char cka=0,ckb=0;
        int i;

        for (i=2;i<len-2;i++) {
            cka+=buff[i]; ckb+=cka;
        }
        return cka==buff[len-2]&&ckb==buff[len-1];
    }
};
