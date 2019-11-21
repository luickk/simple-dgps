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
    static int decode_ubx_msgs(raw_t* raw, std::vector<sat_pos> *satellites_array, unsigned char data)
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

      // decode_rxmsfrbx(raw, satellites_array);
      // decode_rxmrawx(raw, satellites_array);

      switch (type) {
          case ID_RXMRAWX : return decode_rxmrawx(raw, satellites_array);
          case ID_RXMSFRBX: return decode_rxmsfrbx(raw, satellites_array);
      }

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
    if(satellites_array->size()>0){
      for (int i=0; i<satellites_array->size();i++)
      {
        eph_t sp_eph = (*(*satellites_array)[i].eph);
        printf("-------");

        // printf("satno: %d \n pr: %lf \n time_of_ob: %lf \n pseudo_range_observed: %lf \n pseudo_range_basestation_correction: %lf \n",
        // (*satellites_array)[i].satno,(*satellites_array)[i].pseudo_range_observed,(*satellites_array)[i].time_of_eph_observation,(*satellites_array)[i].pseudo_range_observed,(*satellites_array)[i].pseudo_range_basestation_correction);
        // printf("iode: %d \n iodc: %d \n sva: %d \n svh: %d \n week: %d \n code: %d \n flag: %d \n toe: %d \n toc: %d \n ttr: %d \n A: %lf \n e: %lf \n i0: %lf \n OMG0: %lf \n omg: %lf \n M0: %lf \n deln: %lf \n OMGd: %lf \n idot:%lf \n crc: %lf \n crs: %lf \n cuc: %lf \n cus: %lf \n cic: %lf \n cis: %lf \n toes: %lf \n fit: %lf \n f0: %lf \n f1: %lf \n f2: %lf \n tgd: %lf \n Adot: %lf \n ndot: %lf \n ",
        // sp_eph.iode,sp_eph.iodc,sp_eph.sva,sp_eph.svh,sp_eph.week,sp_eph.code,sp_eph.flag,sp_eph.toe,sp_eph.toc,sp_eph.ttr,sp_eph.A,sp_eph.e,sp_eph.i0,sp_eph.OMG0,sp_eph.omg,sp_eph.M0,sp_eph.deln,sp_eph.OMGd,sp_eph.idot,sp_eph.crc,sp_eph.crs,sp_eph.cuc,sp_eph.cus,sp_eph.cic,sp_eph.cis,sp_eph.toes,sp_eph.fit,sp_eph.f0,sp_eph.f1,sp_eph.f2,sp_eph.tgd,sp_eph.Adot,sp_eph.ndot);

        std::cout << "satno: " << (*satellites_array)[i].satno << std::endl;
        std::cout << "pseudo_range_observed: " << (*satellites_array)[i].pseudo_range_observed << std::endl;
        std::cout << "time_of_eph_observation: " << (*satellites_array)[i].time_of_eph_observation << std::endl;
        std::cout << "pseudo_range_observed: " << (*satellites_array)[i].pseudo_range_observed << std::endl;
        std::cout << "pseudo_range_basestation_correction: " << (*satellites_array)[i].pseudo_range_basestation_correction << std::endl;

        std::cout << "iode: " << sp_eph.iode << std::endl;
        std::cout << "iodc: " << sp_eph.iodc << std::endl;
        std::cout << "sva: " << sp_eph.sva << std::endl;
        std::cout << "svh: " << sp_eph.svh << std::endl;
        std::cout << "week: " << sp_eph.week << std::endl;
        std::cout << "code: " << sp_eph.code << std::endl;
        std::cout << "flag: " << sp_eph.flag << std::endl;
        std::cout << "A: " << sp_eph.A << std::endl;
        std::cout << "e: " << sp_eph.e << std::endl;
        std::cout << "i0: " << sp_eph.i0 << std::endl;
        std::cout << "OMG0: " << sp_eph.OMG0 << std::endl;
        std::cout << "omg: " << sp_eph.omg << std::endl;
        std::cout << "M0: " << sp_eph.M0 << std::endl;
        std::cout << "deln: " << sp_eph.deln << std::endl;
        std::cout << "OMGd: " << sp_eph.OMGd << std::endl;
        std::cout << "idot: " << sp_eph.idot << std::endl;
        std::cout << "crc: " << sp_eph.crc << std::endl;
        std::cout << "crs: " << sp_eph.crs << std::endl;
        std::cout << "cuc: " << sp_eph.cuc << std::endl;
        std::cout << "cus: " << sp_eph.cus << std::endl;
        std::cout << "cic: " << sp_eph.cic << std::endl;
        std::cout << "cis: " << sp_eph.cis << std::endl;
        std::cout << "toes: " << sp_eph.toes << std::endl;
        std::cout << "f0: " << sp_eph.f0 << std::endl;
        std::cout << "f1: " << sp_eph.f1 << std::endl;
        std::cout << "f2: " << sp_eph.f2 << std::endl;
        std::cout << "Adot: " << sp_eph.Adot << std::endl;
        std::cout << "ndot: " << sp_eph.ndot  << std::endl;

      }
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
