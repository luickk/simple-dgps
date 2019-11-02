#include <iostream>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ubx_defines.h"
#include "data.h"
#include "nav_data_decoding.cpp"

/* class contains methods to decode raw ubx sfrbx data msgs */

class M8T
{

  static void decode_UBX_RXM_SFRBX_msg(raw_t* raw, unsigned char data)
  {
    /* synchronize frame */
    if (raw->nbyte==0) {
        if (!sync_ubx(raw->buff, data)) printf("msg syncing failed");
        raw->nbyte=2;
        //return 0;
    }

    raw->buff[raw->nbyte++]=data;

    if (raw->nbyte==6) {
        if ((raw->len=U2(raw->buff+4)+8)>MAXRAWLEN) {
            printf("ubx length error: len=%d\n",raw->len);
            raw->nbyte=0;
            // return -1;
        }
    }

    if (raw->nbyte<6||raw->nbyte<raw->len) printf("len err");
    raw->nbyte=0;

    int type=(U1(raw->buff+2)<<8)+U1(raw->buff+3);

    printf("decode_ubx: type=%04x len=%d\n",type,raw->len);

    /* checksum */
    if (!checksum(raw->buff,raw->len)) {
        printf("ubx checksum error: type=%04x len=%d\n",type,raw->len);
        // return -1;
    }

    // decode_rxmsfrbx(raw);

    int prn,sat,sys;
    unsigned char *p=raw->buff+6;

    printf("decode_rxmsfrbx: len=%d\n",raw->len);

    if (!(sys=ubx_sys(U1(p)))) {
        printf("ubx rxmsfrbx sys id error: sys=%d\n",U1(p));
        // return -1;
    }

    prn=U1(p+1)+(sys==SYS_QZS?192:0);
    if (!(sat=satno(sys,prn))) {
        if (sys==SYS_GLO&&prn==255) {
            // return 0; /* suppress error for unknown glo satellite */
        }
        printf("ubx rxmsfrbx sat number error: sys=%d prn=%d\n",sys,prn);
        // return -1;
    }
    switch (sys) {
        case SYS_GPS: decode_nav (raw,sat,8);
        case SYS_QZS: decode_nav (raw,sat,8);
        case SYS_GAL: decode_enav(raw,sat,8);
        case SYS_CMP: decode_cnav(raw,sat,8);
        case SYS_GLO: decode_gnav(raw,sat,8,U1(p+3));
        case SYS_SBS: decode_snav(raw,sat,8);
    }
  }

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
