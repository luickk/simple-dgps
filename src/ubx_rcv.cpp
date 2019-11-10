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
            printf("ubx length error: len=%d\n",raw->len);
            raw->nbyte=0;
            return -1;
        }
      }
      if (raw->nbyte<6||raw->nbyte<raw->len) return 0;
      raw->nbyte=0;

      int type=(U1(raw->buff+2)<<8)+U1(raw->buff+3);

      printf("decode_ubx: type=%04x len=%d\n",type,raw->len);

      /* checksum */
      if (!checksum(raw->buff,raw->len)) {
          printf("ubx checksum error: type=%04x len=%d\n",type,raw->len);
          return -1;
      }

      decode_rxmsfrbx(raw, satellites_array);

      // switch (type) {
      //     case ID_RXMRAW  : return decode_rxmraw  (raw);
      //     case ID_RXMRAWX : return decode_rxmrawx (raw);
      //     case ID_RXMSFRB : return decode_rxmsfrb (raw);
      //     case ID_RXMSFRBX: return decode_rxmsfrbx(raw);
      //     case ID_NAVSOL  : return decode_navsol  (raw);
      //     case ID_NAVTIME : return decode_navtime (raw);
      //     case ID_TRKMEAS : return decode_trkmeas (raw);
      //     case ID_TRKD5   : return decode_trkd5   (raw);
      //     case ID_TRKSFRBX: return decode_trksfrbx(raw);
      // }
      if (raw->outtype) {
        sprintf(raw->msgtype,"UBX 0x%02X 0x%02X (%4d)",type>>8,type&0xF, raw->len);
      }
      return 0;
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
