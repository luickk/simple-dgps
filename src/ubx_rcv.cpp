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
        eph_t sp_eph = (*satellites_array)[i].eph;

        std::cout << "-------------" << std::endl;

        std::cout << "satno: " << (*satellites_array)[i].satno << std::endl;
        std::cout << "pseudo_range_observed: " << (*satellites_array)[i].pseudo_range_observed << std::endl;
        std::cout << "pseudo_range_basestation_correction: " << (*satellites_array)[i].pseudo_range_basestation_correction << std::endl;
        std::cout << "SNR: " << (*satellites_array)[i].SNR << std::endl;
        std::cout << "ecef x pos: " << (*satellites_array)[i].pos[0] << std::endl;
        std::cout << "ecef y pos: " << (*satellites_array)[i].pos[1] << std::endl;
        std::cout << "ecef z pos: " << (*satellites_array)[i].pos[2] << std::endl;

        std::cout << "iode: " << sp_eph.iode << std::endl;
        std::cout << "iodc: " << sp_eph.iodc << std::endl;
        std::cout << "sva: " << sp_eph.sva << std::endl;
        std::cout << "svh: " << sp_eph.svh << std::endl;
        std::cout << "week: " << sp_eph.week << std::endl;
        std::cout << "code: " << sp_eph.code << std::endl;
        std::cout << "flag: " << sp_eph.flag << std::endl;

        std::cout << "toe: " << sp_eph.toe.time << std::endl;
        std::cout << "toc: " << sp_eph.toc.time << std::endl;
        std::cout << "ttr: " << sp_eph.ttr.time << std::endl;

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
        std::cout << "fit: " << sp_eph.fit << std::endl;
        std::cout << "f0: " << sp_eph.f0 << std::endl;
        std::cout << "f1: " << sp_eph.f1 << std::endl;
        std::cout << "f2: " << sp_eph.f2 << std::endl;
        std::cout << "tgd 0: " << sp_eph.tgd[0] << std::endl;
        std::cout << "tgd 1: " << sp_eph.tgd[1] << std::endl;
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
