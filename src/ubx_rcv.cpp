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
          printf("ubx checksum error: type=%04x len=%d\n",type,raw->len);
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
      printf("-------");
      printf("satno: %d \n time_of_ob: %d \n pseudo_range_observed: %d \n pseudo_range_basestation_correction: %d \n",
      (*satellites_array)[i].satno,(*satellites_array)[i].time_of_eph_observation,(*satellites_array)[i].pseudo_range_observed,(*satellites_array)[i].pseudo_range_basestation_correction);
      printf("GPS EPH \n iode: %d \n iodc: %d \n toe: %d \n toc: %d \n ttr: %d \n A: %d \n crc: %d \n f0: %d \n",
      (*satellites_array)[i].eph->iode,(*satellites_array)[i].eph->iodc,(*satellites_array)[i].eph->toe,(*satellites_array)[i].eph->toc,(*satellites_array)[i].eph->ttr,(*satellites_array)[i].eph->A,(*satellites_array)[i].eph->crc,(*satellites_array)[i].eph->f0);
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
