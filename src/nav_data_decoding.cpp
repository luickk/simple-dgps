#define PREAMB_CNAV 0x8B

#define SQR(x)      ((x)*(x))

static const double gpst0[]={1980,1, 6,0,0,0}; /* gps time reference */
static const double gst0 []={1999,8,22,0,0,0}; /* galileo system time reference */
static const double bdt0 []={2006,1, 1,0,0,0}; /* beidou time reference */

static double timeoffset_=0.0;        /* time offset (s) */

static double leaps[MAXLEAPS+1][7]={ /* leap seconds (y,m,d,h,m,s,utc-gpst) */
    {2017,1,1,0,0,0,-18},
    {2015,7,1,0,0,0,-17},
    {2012,7,1,0,0,0,-16},
    {2009,1,1,0,0,0,-15},
    {2006,1,1,0,0,0,-14},
    {1999,1,1,0,0,0,-13},
    {1997,7,1,0,0,0,-12},
    {1996,1,1,0,0,0,-11},
    {1994,7,1,0,0,0,-10},
    {1993,7,1,0,0,0, -9},
    {1992,7,1,0,0,0, -8},
    {1991,1,1,0,0,0, -7},
    {1990,1,1,0,0,0, -6},
    {1988,1,1,0,0,0, -5},
    {1985,7,1,0,0,0, -4},
    {1983,7,1,0,0,0, -3},
    {1982,7,1,0,0,0, -2},
    {1981,7,1,0,0,0, -1},
    {0}
};
static unsigned char obsfreqs[]={
    /* 1:L1/E1, 2:L2/B1, 3:L5/E5a/L3, 4:L6/LEX/B3, 5:E5b/B2, 6:E5(a+b), 7:S */
    0, 1, 1, 1, 1,  1, 1, 1, 1, 1, /*  0- 9 */
    1, 1, 1, 1, 2,  2, 2, 2, 2, 2, /* 10-19 */
    2, 2, 2, 2, 3,  3, 3, 5, 5, 5, /* 20-29 */
    4, 4, 4, 4, 4,  4, 4, 6, 6, 6, /* 30-39 */
    2, 2, 4, 4, 3,  3, 3, 1, 1, 3, /* 40-49 */
    3, 3, 7, 7, 7,  7, 0, 0, 0, 0  /* 50-59 */
};

static const unsigned int tbl_CRC24Q[]={
    0x000000,0x864CFB,0x8AD50D,0x0C99F6,0x93E6E1,0x15AA1A,0x1933EC,0x9F7F17,
    0xA18139,0x27CDC2,0x2B5434,0xAD18CF,0x3267D8,0xB42B23,0xB8B2D5,0x3EFE2E,
    0xC54E89,0x430272,0x4F9B84,0xC9D77F,0x56A868,0xD0E493,0xDC7D65,0x5A319E,
    0x64CFB0,0xE2834B,0xEE1ABD,0x685646,0xF72951,0x7165AA,0x7DFC5C,0xFBB0A7,
    0x0CD1E9,0x8A9D12,0x8604E4,0x00481F,0x9F3708,0x197BF3,0x15E205,0x93AEFE,
    0xAD50D0,0x2B1C2B,0x2785DD,0xA1C926,0x3EB631,0xB8FACA,0xB4633C,0x322FC7,
    0xC99F60,0x4FD39B,0x434A6D,0xC50696,0x5A7981,0xDC357A,0xD0AC8C,0x56E077,
    0x681E59,0xEE52A2,0xE2CB54,0x6487AF,0xFBF8B8,0x7DB443,0x712DB5,0xF7614E,
    0x19A3D2,0x9FEF29,0x9376DF,0x153A24,0x8A4533,0x0C09C8,0x00903E,0x86DCC5,
    0xB822EB,0x3E6E10,0x32F7E6,0xB4BB1D,0x2BC40A,0xAD88F1,0xA11107,0x275DFC,
    0xDCED5B,0x5AA1A0,0x563856,0xD074AD,0x4F0BBA,0xC94741,0xC5DEB7,0x43924C,
    0x7D6C62,0xFB2099,0xF7B96F,0x71F594,0xEE8A83,0x68C678,0x645F8E,0xE21375,
    0x15723B,0x933EC0,0x9FA736,0x19EBCD,0x8694DA,0x00D821,0x0C41D7,0x8A0D2C,
    0xB4F302,0x32BFF9,0x3E260F,0xB86AF4,0x2715E3,0xA15918,0xADC0EE,0x2B8C15,
    0xD03CB2,0x567049,0x5AE9BF,0xDCA544,0x43DA53,0xC596A8,0xC90F5E,0x4F43A5,
    0x71BD8B,0xF7F170,0xFB6886,0x7D247D,0xE25B6A,0x641791,0x688E67,0xEEC29C,
    0x3347A4,0xB50B5F,0xB992A9,0x3FDE52,0xA0A145,0x26EDBE,0x2A7448,0xAC38B3,
    0x92C69D,0x148A66,0x181390,0x9E5F6B,0x01207C,0x876C87,0x8BF571,0x0DB98A,
    0xF6092D,0x7045D6,0x7CDC20,0xFA90DB,0x65EFCC,0xE3A337,0xEF3AC1,0x69763A,
    0x578814,0xD1C4EF,0xDD5D19,0x5B11E2,0xC46EF5,0x42220E,0x4EBBF8,0xC8F703,
    0x3F964D,0xB9DAB6,0xB54340,0x330FBB,0xAC70AC,0x2A3C57,0x26A5A1,0xA0E95A,
    0x9E1774,0x185B8F,0x14C279,0x928E82,0x0DF195,0x8BBD6E,0x872498,0x016863,
    0xFAD8C4,0x7C943F,0x700DC9,0xF64132,0x693E25,0xEF72DE,0xE3EB28,0x65A7D3,
    0x5B59FD,0xDD1506,0xD18CF0,0x57C00B,0xC8BF1C,0x4EF3E7,0x426A11,0xC426EA,
    0x2AE476,0xACA88D,0xA0317B,0x267D80,0xB90297,0x3F4E6C,0x33D79A,0xB59B61,
    0x8B654F,0x0D29B4,0x01B042,0x87FCB9,0x1883AE,0x9ECF55,0x9256A3,0x141A58,
    0xEFAAFF,0x69E604,0x657FF2,0xE33309,0x7C4C1E,0xFA00E5,0xF69913,0x70D5E8,
    0x4E2BC6,0xC8673D,0xC4FECB,0x42B230,0xDDCD27,0x5B81DC,0x57182A,0xD154D1,
    0x26359F,0xA07964,0xACE092,0x2AAC69,0xB5D37E,0x339F85,0x3F0673,0xB94A88,
    0x87B4A6,0x01F85D,0x0D61AB,0x8B2D50,0x145247,0x921EBC,0x9E874A,0x18CBB1,
    0xE37B16,0x6537ED,0x69AE1B,0xEFE2E0,0x709DF7,0xF6D10C,0xFA48FA,0x7C0401,
    0x42FA2F,0xC4B6D4,0xC82F22,0x4E63D9,0xD11CCE,0x575035,0x5BC9C3,0xDD8538
};
/* test hamming code of glonass ephemeris string -------------------------------
* test hamming code of glonass ephemeris string (ref [2] 4.7)
* args   : unsigned char *buff I glonass navigation data string bits in frame
*                                with hamming
*                                  buff[ 0]: string bit 85-78
*                                  buff[ 1]: string bit 77-70
*                                  ...
*                                  buff[10]: string bit  5- 1 (0 padded)
* return : status (1:ok,0:error)
*-----------------------------------------------------------------------------*/
extern int test_glostr(const unsigned char *buff)
{
    static const unsigned char xor_8bit[256]={ /* xor of 8 bits */
        0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
        1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
        1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
        0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
        1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,
        0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
        0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,
        1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0
    };
    static const unsigned char mask_hamming[][12]={ /* mask of hamming codes */
        {0x55,0x55,0x5A,0xAA,0xAA,0xAA,0xB5,0x55,0x6A,0xD8,0x08},
        {0x66,0x66,0x6C,0xCC,0xCC,0xCC,0xD9,0x99,0xB3,0x68,0x10},
        {0x87,0x87,0x8F,0x0F,0x0F,0x0F,0x1E,0x1E,0x3C,0x70,0x20},
        {0x07,0xF8,0x0F,0xF0,0x0F,0xF0,0x1F,0xE0,0x3F,0x80,0x40},
        {0xF8,0x00,0x0F,0xFF,0xF0,0x00,0x1F,0xFF,0xC0,0x00,0x80},
        {0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x01,0x00},
        {0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00},
        {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8}
    };
    unsigned char cs;
    int i,j,n=0;

    for (i=0;i<8;i++) {
        for (j=0,cs=0;j<11;j++) {
            cs^=xor_8bit[buff[j]&mask_hamming[i][j]];
        }
        if (cs) n++;
    }
    return n==0||(n==2&&cs);
}
/* crc-24q parity --------------------------------------------------------------
* compute crc-24q parity for sbas, rtcm3
* args   : unsigned char *buff I data
*          int    len    I      data length (bytes)
* return : crc-24Q parity
* notes  : see reference [2] A.4.3.3 Parity
*-----------------------------------------------------------------------------*/
extern unsigned int rtk_crc24q(const unsigned char *buff, int len)
{
    unsigned int crc=0;
    int i;


    printf("rtk_crc24q: len=%d\n",len);

    for (i=0;i<len;i++) crc=((crc<<8)&0xFFFFFF)^tbl_CRC24Q[(crc>>16)^buff[i]];
    return crc;
}


  /* satellite number to satellite system ----------------------------------------
  * convert satellite number to satellite system
  * args   : int    sat       I   satellite number (1-MAXSAT)
  *          int    *prn      IO  satellite prn/slot number (NULL: no output)
  * return : satellite system (SYS_GPS,SYS_GLO,...)
  *-----------------------------------------------------------------------------*/
  extern int satsys(int sat, int *prn)
  {
      int sys=SYS_NONE;
      if (sat<=0||MAXSAT<sat) sat=0;
      else if (sat<=NSATGPS) {
          sys=SYS_GPS; sat+=MINPRNGPS-1;
      }
      else if ((sat-=NSATGPS)<=NSATGLO) {
          sys=SYS_GLO; sat+=MINPRNGLO-1;
      }
      else if ((sat-=NSATGLO)<=NSATGAL) {
          sys=SYS_GAL; sat+=MINPRNGAL-1;
      }
      else if ((sat-=NSATGAL)<=NSATQZS) {
          sys=SYS_QZS; sat+=MINPRNQZS-1;
      }
      else if ((sat-=NSATQZS)<=NSATCMP) {
          sys=SYS_CMP; sat+=MINPRNCMP-1;
      }
      else if ((sat-=NSATCMP)<=NSATIRN) {
          sys=SYS_IRN; sat+=MINPRNIRN-1;
      }
      else if ((sat-=NSATIRN)<=NSATLEO) {
          sys=SYS_LEO; sat+=MINPRNLEO-1;
      }
      else if ((sat-=NSATLEO)<=NSATSBS) {
          sys=SYS_SBS; sat+=MINPRNSBS-1;
      }
      else sat=0;
      if (prn) *prn=sat;
      return sys;
  }
  /* satellite id to satellite number --------------------------------------------
  * convert satellite id to satellite number
  * args   : char   *id       I   satellite id (nn,Gnn,Rnn,Enn,Jnn,Cnn,Inn or Snn)
  * return : satellite number (0: error)
  * notes  : 120-142 and 193-199 are also recognized as sbas and qzss
  *-----------------------------------------------------------------------------*/
  extern int satid2no(const char *id)
  {
      int sys,prn;
      char code;

      if (sscanf(id,"%d",&prn)==1) {
          if      (MINPRNGPS<=prn&&prn<=MAXPRNGPS) sys=SYS_GPS;
          else if (MINPRNSBS<=prn&&prn<=MAXPRNSBS) sys=SYS_SBS;
          else if (MINPRNQZS<=prn&&prn<=MAXPRNQZS) sys=SYS_QZS;
          else return 0;
          return satno(sys,prn);
      }
      if (sscanf(id,"%c%d",&code,&prn)<2) return 0;

      switch (code) {
          case 'G': sys=SYS_GPS; prn+=MINPRNGPS-1; break;
          case 'R': sys=SYS_GLO; prn+=MINPRNGLO-1; break;
          case 'E': sys=SYS_GAL; prn+=MINPRNGAL-1; break;
          case 'J': sys=SYS_QZS; prn+=MINPRNQZS-1; break;
          case 'C': sys=SYS_CMP; prn+=MINPRNCMP-1; break;
          case 'I': sys=SYS_IRN; prn+=MINPRNIRN-1; break;
          case 'L': sys=SYS_LEO; prn+=MINPRNLEO-1; break;
          case 'S': sys=SYS_SBS; prn+=100; break;
          default: return 0;
      }
      return satno(sys,prn);
  }
  /* satellite number to satellite id --------------------------------------------
  * convert satellite number to satellite id
  * args   : int    sat       I   satellite number
  *          char   *id       O   satellite id (Gnn,Rnn,Enn,Jnn,Cnn,Inn or nnn)
  * return : none
  *-----------------------------------------------------------------------------*/
  extern void satno2id(int sat, char *id)
  {
      int prn;
      switch (satsys(sat,&prn)) {
          case SYS_GPS: sprintf(id,"G%02d",prn-MINPRNGPS+1); return;
          case SYS_GLO: sprintf(id,"R%02d",prn-MINPRNGLO+1); return;
          case SYS_GAL: sprintf(id,"E%02d",prn-MINPRNGAL+1); return;
          case SYS_QZS: sprintf(id,"J%02d",prn-MINPRNQZS+1); return;
          case SYS_CMP: sprintf(id,"C%02d",prn-MINPRNCMP+1); return;
          case SYS_IRN: sprintf(id,"I%02d",prn-MINPRNIRN+1); return;
          case SYS_LEO: sprintf(id,"L%02d",prn-MINPRNLEO+1); return;
          case SYS_SBS: sprintf(id,"%03d" ,prn); return;
      }
      strcpy(id,"");
  }
  /* test excluded satellite -----------------------------------------------------
  * test excluded satellite
  * args   : int    sat       I   satellite number
  *          double var       I   variance of ephemeris (m^2)
  *          int    svh       I   sv health flag
  *          prcopt_t *opt    I   processing options (NULL: not used)
  * return : status (1:excluded,0:not excluded)
  *-----------------------------------------------------------------------------*/
  extern int satexclude(int sat, double var, int svh, const prcopt_t *opt)
  {
      int sys=satsys(sat,NULL);

      if (svh<0) return 1; /* ephemeris unavailable */

      if (opt) {
          if (opt->exsats[sat-1]==1) return 1; /* excluded satellite */
          if (opt->exsats[sat-1]==2) return 0; /* included satellite */
          if (!(sys&opt->navsys)) return 1; /* unselected sat sys */
      }
      if (sys==SYS_QZS) svh&=0xFE; /* mask QZSS LEX health */
      if (svh) {
          printf("unhealthy satellite: sat=%3d svh=%02X\n",sat,svh);
          return 1;
      }
      if (var>MAX_VAR_EPH) {
        printf("invalid ura satellite: sat=%3d ura=%.2f\n",sat,sqrt(var));
          return 1;
      }
      return 0;
  }
  /* test SNR mask ---------------------------------------------------------------
  * test SNR mask
  * args   : int    base      I   rover or base-station (0:rover,1:base station)
  *          int    freq      I   frequency (0:L1,1:L2,2:L3,...)
  *          double el        I   elevation angle (rad)
  *          double snr       I   C/N0 (dBHz)
  *          snrmask_t *mask  I   SNR mask
  * return : status (1:masked,0:unmasked)
  *-----------------------------------------------------------------------------*/
  extern int testsnr(int base, int freq, double el, double snr,
                     const snrmask_t *mask)
  {
      double minsnr,a;
      int i;

      if (!mask->ena[base]||freq<0||freq>=NFREQ) return 0;

      a=(el*R2D+5.0)/10.0;
      i=(int)floor(a); a-=i;
      if      (i<1) minsnr=mask->mask[freq][0];
      else if (i>8) minsnr=mask->mask[freq][8];
      else minsnr=(1.0-a)*mask->mask[freq][i-1]+a*mask->mask[freq][i];

      return snr<minsnr;
  }
  /* obs type string to obs code -------------------------------------------------
  * convert obs code type string to obs code
  * args   : char   *str   I      obs code string ("1C","1P","1Y",...)
  *          int    *freq  IO     frequency (1:L1,2:L2,3:L5,4:L6,5:L7,6:L8,0:err)
  *                               (NULL: no output)
  * return : obs code (CODE_???)
  * notes  : obs codes are based on reference [6] and qzss extension
  *-----------------------------------------------------------------------------*/
  extern unsigned char obs2code(const char *obs, int *freq)
  {
      int i;
      if (freq) *freq=0;
      for (i=1;*obscodes[i];i++) {
          if (strcmp(obscodes[i],obs)) continue;
          if (freq) *freq=obsfreqs[i];
          return (unsigned char)i;
      }
      return CODE_NONE;
  }
  /* obs code to obs code string -------------------------------------------------
  * convert obs code to obs code string
  * args   : unsigned char code I obs code (CODE_???)
  *          int    *freq  IO     frequency (NULL: no output)
  *                               (1:L1/E1, 2:L2/B1, 3:L5/E5a/L3, 4:L6/LEX/B3,
                                   5:E5b/B2, 6:E5(a+b), 7:S)
  * return : obs code string ("1C","1P","1P",...)
  * notes  : obs codes are based on reference [6] and qzss extension
  *-----------------------------------------------------------------------------*/
  extern char *code2obs(unsigned char code, int *freq)
  {
      if (freq) *freq=0;
      if (code<=CODE_NONE||MAXCODE<code) return 0;
      if (freq) *freq=obsfreqs[code];
      return obscodes[code];
  }
  /* set code priority -----------------------------------------------------------
  * set code priority for multiple codes in a frequency
  * args   : int    sys     I     system (or of SYS_???)
  *          int    freq    I     frequency (1:L1,2:L2,3:L5,4:L6,5:L7,6:L8,7:L9)
  *          char   *pri    I     priority of codes (series of code characters)
  *                               (higher priority precedes lower)
  * return : none
  *-----------------------------------------------------------------------------*/
  extern void setcodepri(int sys, int freq, const char *pri)
  {
      printf("setcodepri:sys=%d freq=%d pri=%s\n",sys,freq,pri);

      if (freq<=0||MAXFREQ<freq) return;
      if (sys&SYS_GPS) strcpy(codepris[0][freq-1],pri);
      if (sys&SYS_GLO) strcpy(codepris[1][freq-1],pri);
      if (sys&SYS_GAL) strcpy(codepris[2][freq-1],pri);
      if (sys&SYS_QZS) strcpy(codepris[3][freq-1],pri);
      if (sys&SYS_SBS) strcpy(codepris[4][freq-1],pri);
      if (sys&SYS_CMP) strcpy(codepris[5][freq-1],pri);
      if (sys&SYS_IRN) strcpy(codepris[6][freq-1],pri);
  }
  /* get code priority -----------------------------------------------------------
  * get code priority for multiple codes in a frequency
  * args   : int    sys     I     system (SYS_???)
  *          unsigned char code I obs code (CODE_???)
  *          char   *opt    I     code options (NULL:no option)
  * return : priority (15:highest-1:lowest,0:error)
  *-----------------------------------------------------------------------------*/
  extern int getcodepri(int sys, unsigned char code, const char *opt)
  {
      const char *p,*optstr;
      char *obs,str[8]="";
      int i,j;

      switch (sys) {
          case SYS_GPS: i=0; optstr="-GL%2s"; break;
          case SYS_GLO: i=1; optstr="-RL%2s"; break;
          case SYS_GAL: i=2; optstr="-EL%2s"; break;
          case SYS_QZS: i=3; optstr="-JL%2s"; break;
          case SYS_SBS: i=4; optstr="-SL%2s"; break;
          case SYS_CMP: i=5; optstr="-CL%2s"; break;
          case SYS_IRN: i=6; optstr="-IL%2s"; break;
          default: return 0;
      }
      obs=code2obs(code,&j);

      /* parse code options */
      for (p=opt;p&&(p=strchr(p,'-'));p++) {
          if (sscanf(p,optstr,str)<1||str[0]!=obs[0]) continue;
          return str[1]==obs[1]?15:0;
      }
      /* search code priority */
      return (p=strchr(codepris[i][j-1],obs[1]))?14-(int)(p-codepris[i][j-1]):0;
  }
  /* satellite system+prn/slot number to satellite number ------------------------
  * convert satellite system+prn/slot number to satellite number
  * args   : int    sys       I   satellite system (SYS_GPS,SYS_GLO,...)
  *          int    prn       I   satellite prn/slot number
  * return : satellite number (0:error)
  *-----------------------------------------------------------------------------*/
  extern int satno(int sys, int prn)
  {
      if (prn<=0) return 0;
      switch (sys) {
          case SYS_GPS:
              if (prn<MINPRNGPS||MAXPRNGPS<prn) return 0;
              return prn-MINPRNGPS+1;
          case SYS_GLO:
              if (prn<MINPRNGLO||MAXPRNGLO<prn) return 0;
              return NSATGPS+prn-MINPRNGLO+1;
          case SYS_GAL:
              if (prn<MINPRNGAL||MAXPRNGAL<prn) return 0;
              return NSATGPS+NSATGLO+prn-MINPRNGAL+1;
          case SYS_QZS:
              if (prn<MINPRNQZS||MAXPRNQZS<prn) return 0;
              return NSATGPS+NSATGLO+NSATGAL+prn-MINPRNQZS+1;
          case SYS_CMP:
              if (prn<MINPRNCMP||MAXPRNCMP<prn) return 0;
              return NSATGPS+NSATGLO+NSATGAL+NSATQZS+prn-MINPRNCMP+1;
          case SYS_IRN:
              if (prn<MINPRNIRN||MAXPRNIRN<prn) return 0;
              return NSATGPS+NSATGLO+NSATGAL+NSATQZS+NSATCMP+prn-MINPRNIRN+1;
          case SYS_LEO:
              if (prn<MINPRNLEO||MAXPRNLEO<prn) return 0;
              return NSATGPS+NSATGLO+NSATGAL+NSATQZS+NSATCMP+NSATIRN+
                     prn-MINPRNLEO+1;
          case SYS_SBS:
              if (prn<MINPRNSBS||MAXPRNSBS<prn) return 0;
              return NSATGPS+NSATGLO+NSATGAL+NSATQZS+NSATCMP+NSATIRN+NSATLEO+
                     prn-MINPRNSBS+1;
      }
      return 0;
  }

/* extract unsigned/signed bits ------------------------------------------------
* extract unsigned/signed bits from byte data
* args   : unsigned char *buff I byte data
*          int    pos    I      bit position from start of data (bits)
*          int    len    I      bit length (bits) (len<=32)
* return : extracted unsigned/signed bits
*-----------------------------------------------------------------------------*/
extern unsigned int getbitu(const unsigned char *buff, int pos, int len)
{
    unsigned int bits=0;
    int i;
    for (i=pos;i<pos+len;i++) bits=(bits<<1)+((buff[i/8]>>(7-i%8))&1u);
    return bits;
}
/* save subframe -------------------------------------------------------------*/
static int save_subfrm(int sat, raw_t *raw)
{
    unsigned char *p=raw->buff+6,*q;
    int i,j,n,id=(U4(p+6)>>2)&0x7;

    printf("save_subfrm: sat=%2d id=%d\n",sat,id);

    if (id<1||5<id) return 0;

    q=raw->subfrm[sat-1]+(id-1)*30;

    for (i=n=0,p+=2;i<10;i++,p+=4) {
        for (j=23;j>=0;j--) {
            *q=(*q<<1)+((U4(p)>>j)&1); if (++n%8==0) q++;
        }
    }
    return id;
}
/* convert calendar day/time to time -------------------------------------------
* convert calendar day/time to gtime_t struct
* args   : double *ep       I   day/time {year,month,day,hour,min,sec}
* return : gtime_t struct
* notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
*-----------------------------------------------------------------------------*/
extern gtime_t epoch2time(const double *ep)
{
    const int doy[]={1,32,60,91,121,152,182,213,244,274,305,335};
    gtime_t time={0};
    int days,sec,year=(int)ep[0],mon=(int)ep[1],day=(int)ep[2];

    if (year<1970||2099<year||mon<1||12<mon) return time;

    /* leap year if year%4==0 in 1901-2099 */
    days=(year-1970)*365+(year-1969)/4+doy[mon-1]+day-2+(year%4==0&&mon>=3?1:0);
    sec=(int)floor(ep[5]);
    time.time=(time_t)days*86400+(int)ep[3]*3600+(int)ep[4]*60+sec;
    time.sec=ep[5]-sec;
    return time;
}
/* galileo system time to time -------------------------------------------------
* convert week and tow in galileo system time (gst) to gtime_t struct
* args   : int    week      I   week number in gst
*          double sec       I   time of week in gst (s)
* return : gtime_t struct
*-----------------------------------------------------------------------------*/
extern gtime_t gst2time(int week, double sec)
{
    gtime_t t=epoch2time(gst0);

    if (sec<-1E9||1E9<sec) sec=0.0;
    t.time+=(time_t)86400*7*week+(int)sec;
    t.sec=sec-(int)sec;
    return t;
}

/* time to gps time ------------------------------------------------------------
* convert gtime_t struct to week and tow in gps time
* args   : gtime_t t        I   gtime_t struct
*          int    *week     IO  week number in gps time (NULL: no output)
* return : time of week in gps time (s)
*-----------------------------------------------------------------------------*/
extern double time2gpst(gtime_t t, int *week)
{
    gtime_t t0=epoch2time(gpst0);
    time_t sec=t.time-t0.time;
    int w=(int)(sec/(86400*7));

    if (week) *week=w;
    return (double)(sec-(double)w*86400*7)+t.sec;
}
/* 8-bit week -> full week ---------------------------------------------------*/
static void adj_utcweek(gtime_t time, double *utc)
{
    int week;

    if (utc[3]>=256.0) return;
    time2gpst(time,&week);
    utc[3]+=week/256*256;
    if      (utc[3]<week-128) utc[3]+=256.0;
    else if (utc[3]>week+128) utc[3]-=256.0;
}





/* time difference -------------------------------------------------------------
* difference between gtime_t structs
* args   : gtime_t t1,t2    I   gtime_t structs
* return : time difference (t1-t2) (s)
*-----------------------------------------------------------------------------*/
extern double timediff(gtime_t t1, gtime_t t2)
{
    return difftime(t1.time,t2.time)+t1.sec-t2.sec;
}

/* add time --------------------------------------------------------------------
* add time to gtime_t struct
* args   : gtime_t t        I   gtime_t struct
*          double sec       I   time to add (s)
* return : gtime_t struct (t+sec)
*-----------------------------------------------------------------------------*/
extern gtime_t timeadd(gtime_t t, double sec)
{
    double tt;

    t.sec+=sec; tt=floor(t.sec); t.time+=(int)tt; t.sec-=tt;
    return t;
}

/* gps time to time ------------------------------------------------------------
* convert week and tow in gps time to gtime_t struct
* args   : int    week      I   week number in gps time
*          double sec       I   time of week in gps time (s)
* return : gtime_t struct
*-----------------------------------------------------------------------------*/
extern gtime_t gpst2time(int week, double sec)
{
    gtime_t t=epoch2time(gpst0);

    if (sec<-1E9||1E9<sec) sec=0.0;
    t.time+=(time_t)86400*7*week+(int)sec;
    t.sec=sec-(int)sec;
    return t;
}

/* utc to gpstime --------------------------------------------------------------
* convert utc to gpstime considering leap seconds
* args   : gtime_t t        I   time expressed in utc
* return : time expressed in gpstime
* notes  : ignore slight time offset under 100 ns
*-----------------------------------------------------------------------------*/
extern gtime_t utc2gpst(gtime_t t)
{
    int i;

    for (i=0;leaps[i][0]>0;i++) {
        if (timediff(t,epoch2time(leaps[i]))>=0.0) return timeadd(t,-leaps[i][6]);
    }
    return t;
}



/* time to calendar day/time ---------------------------------------------------
* convert gtime_t struct to calendar day/time
* args   : gtime_t t        I   gtime_t struct
*          double *ep       O   day/time {year,month,day,hour,min,sec}
* return : none
* notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
*-----------------------------------------------------------------------------*/
extern void time2epoch(gtime_t t, double *ep)
{
    const int mday[]={ /* # of days in a month */
        31,28,31,30,31,30,31,31,30,31,30,31,31,28,31,30,31,30,31,31,30,31,30,31,
        31,29,31,30,31,30,31,31,30,31,30,31,31,28,31,30,31,30,31,31,30,31,30,31
    };
    int days,sec,mon,day;

    /* leap year if year%4==0 in 1901-2099 */
    days=(int)(t.time/86400);
    sec=(int)(t.time-(time_t)days*86400);
    for (day=days%1461,mon=0;mon<48;mon++) {
        if (day>=mday[mon]) day-=mday[mon]; else break;
    }
    ep[0]=1970+days/1461*4+mon/12; ep[1]=mon%12+1; ep[2]=day+1;
    ep[3]=sec/3600; ep[4]=sec%3600/60; ep[5]=sec%60+t.sec;
}

/* gpstime to utc --------------------------------------------------------------
* convert gpstime to utc considering leap seconds
* args   : gtime_t t        I   time expressed in gpstime
* return : time expressed in utc
* notes  : ignore slight time offset under 100 ns
*-----------------------------------------------------------------------------*/
extern gtime_t gpst2utc(gtime_t t)
{
    gtime_t tu;
    int i;

    for (i=0;leaps[i][0]>0;i++) {
        tu=timeadd(t,leaps[i][6]);
        if (timediff(tu,epoch2time(leaps[i]))>=0.0) return tu;
    }
    return t;
}

extern gtime_t timeget(void)
{
    gtime_t time;
    double ep[6]={0};
#ifdef WIN32
    SYSTEMTIME ts;

    GetSystemTime(&ts); /* utc */
    ep[0]=ts.wYear; ep[1]=ts.wMonth;  ep[2]=ts.wDay;
    ep[3]=ts.wHour; ep[4]=ts.wMinute; ep[5]=ts.wSecond+ts.wMilliseconds*1E-3;
#else
    struct timeval tv;
    struct tm *tt;

    if (!gettimeofday(&tv,NULL)&&(tt=gmtime(&tv.tv_sec))) {
        ep[0]=tt->tm_year+1900; ep[1]=tt->tm_mon+1; ep[2]=tt->tm_mday;
        ep[3]=tt->tm_hour; ep[4]=tt->tm_min; ep[5]=tt->tm_sec+tv.tv_usec*1E-6;
    }
#endif
    time=epoch2time(ep);

#ifdef CPUTIME_IN_GPST /* cputime operated in gpst */
    time=gpst2utc(time);
#endif
    return timeadd(time,timeoffset_);
}


/* set unsigned/signed bits ----------------------------------------------------
* set unsigned/signed bits to byte data
* args   : unsigned char *buff IO byte data
*          int    pos    I      bit position from start of data (bits)
*          int    len    I      bit length (bits) (len<=32)
*         (unsigned) int I      unsigned/signed data
* return : none
*-----------------------------------------------------------------------------*/
extern void setbitu(unsigned char *buff, int pos, int len, unsigned int data)
{
    unsigned int mask=1u<<(len-1);
    int i;
    if (len<=0||32<len) return;
    for (i=pos;i<pos+len;i++,mask>>=1) {
        if (data&mask) buff[i/8]|=1u<<(7-i%8); else buff[i/8]&=~(1u<<(7-i%8));
    }
}
extern void setbits(unsigned char *buff, int pos, int len, int data)
{
    if (data<0) data|=1<<(len-1); else data&=~(1<<(len-1)); /* set sign bit */
    setbitu(buff,pos,len,(unsigned int)data);
}

/* get two component bits ----------------------------------------------------*/
static unsigned int getbitu2(const unsigned char *buff, int p1, int l1, int p2,
                             int l2)
{
    return (getbitu(buff,p1,l1)<<l2)+getbitu(buff,p2,l2);
}

/* adjust gps week number ------------------------------------------------------
* adjust gps week number using cpu time
* args   : int   week       I   not-adjusted gps week number
* return : adjusted gps week number
*-----------------------------------------------------------------------------*/
extern int adjgpsweek(int week)
{
    int w;
    (void)time2gpst(utc2gpst(timeget()),&w);
    if (w<1560) w=1560; /* use 2009/12/1 if time is earlier than 2009/12/1 */
    return week+(w-week+512)/1024*1024;
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

extern int getbits(const unsigned char *buff, int pos, int len)
{
    unsigned int bits=getbitu(buff,pos,len);
    if (len<=0||32<=len||!(bits&(1u<<(len-1)))) return (int)bits;
    return (int)(bits|(~0u<<len)); /* extend sign */
}
static int getbits2(const unsigned char *buff, int p1, int l1, int p2, int l2)
{
    if (getbitu(buff,p1,1))
        return (int)((getbits(buff,p1,l1)<<l2)+getbitu(buff,p2,l2));
    else
        return (int)getbitu2(buff,p1,l1,p2,l2);
}
/* get three component bits --------------------------------------------------*/
static unsigned int getbitu3(const unsigned char *buff, int p1, int l1, int p2,
                             int l2, int p3, int l3)
{
    return (getbitu(buff,p1,l1)<<(l2+l3))+(getbitu(buff,p2,l2)<<l3)+
            getbitu(buff,p3,l3);
}
static int getbits3(const unsigned char *buff, int p1, int l1, int p2, int l2,
                    int p3, int l3)
{
    if (getbitu(buff,p1,1))
        return (int)((getbits(buff,p1,l1)<<(l2+l3))+
                   (getbitu(buff,p2,l2)<<l3)+getbitu(buff,p3,l3));
    else
        return (int)getbitu3(buff,p1,l1,p2,l2,p3,l3);
}

/* beidou time (bdt) to time ---------------------------------------------------
* convert week and tow in beidou time (bdt) to gtime_t struct
* args   : int    week      I   week number in bdt
*          double sec       I   time of week in bdt (s)
* return : gtime_t struct
*-----------------------------------------------------------------------------*/
extern gtime_t bdt2time(int week, double sec)
{
    gtime_t t=epoch2time(bdt0);

    if (sec<-1E9||1E9<sec) sec=0.0;
    t.time+=(time_t)86400*7*week+(int)sec;
    t.sec=sec-(int)sec;
    return t;
}

/* bdt to gpstime --------------------------------------------------------------
* convert bdt (beidou navigation satellite system time) to gpstime
* args   : gtime_t t        I   time expressed in bdt
* return : time expressed in gpstime
* notes  : see gpst2bdt()
*-----------------------------------------------------------------------------*/
extern gtime_t bdt2gpst(gtime_t t)
{
    return timeadd(t,14.0);
}
/* merge two components ------------------------------------------------------*/
static unsigned int merge_two_u(unsigned int a, unsigned int b, int n)
{
    return (a<<n)+b;
}
/* decode BeiDou D1 ephemeris --------------------------------------------------
* decode BeiDou D1 ephemeris (IGSO/MEO satellites) (ref [3] 5.2)
* args   : unsigned char *buff I beidou D1 subframe bits
*                                  buff[ 0- 37]: subframe 1 (300 bits)
*                                  buff[38- 75]: subframe 2
*                                  buff[76-113]: subframe 3
*          eph_t    *eph    IO  ephemeris structure
* return : status (1:ok,0:error)
*-----------------------------------------------------------------------------*/
extern int decode_bds_d1(const unsigned char *buff, eph_t *eph)
{
    double toc_bds,sqrtA;
    unsigned int toe1,toe2,sow1,sow2,sow3;
    int i,frn1,frn2,frn3;

    printf("decode_bds_d1:\n");

    i=8*38*0; /* subframe 1 */
    frn1       =getbitu (buff,i+ 15, 3);
    sow1       =getbitu2(buff,i+ 18, 8,i+30,12);
    eph->svh   =getbitu (buff,i+ 42, 1); /* SatH1 */
    eph->iodc  =getbitu (buff,i+ 43, 5); /* AODC */
    eph->sva   =getbitu (buff,i+ 48, 4);
    eph->week  =getbitu (buff,i+ 60,13); /* week in BDT */
    toc_bds    =getbitu2(buff,i+ 73, 9,i+ 90, 8)*8.0;
    eph->tgd[0]=getbits (buff,i+ 98,10)*0.1*1E-9;
    eph->tgd[1]=getbits2(buff,i+108, 4,i+120, 6)*0.1*1E-9;
    eph->f2    =getbits (buff,i+214,11)*P2_66;
    eph->f0    =getbits2(buff,i+225, 7,i+240,17)*P2_33;
    eph->f1    =getbits2(buff,i+257, 5,i+270,17)*P2_50;
    eph->iode  =getbitu (buff,i+287, 5); /* AODE */

    i=8*38*1; /* subframe 2 */
    frn2       =getbitu (buff,i+ 15, 3);
    sow2       =getbitu2(buff,i+ 18, 8,i+30,12);
    eph->deln  =getbits2(buff,i+ 42,10,i+ 60, 6)*P2_43*SC2RAD;
    eph->cuc   =getbits2(buff,i+ 66,16,i+ 90, 2)*P2_31;
    eph->M0    =getbits2(buff,i+ 92,20,i+120,12)*P2_31*SC2RAD;
    eph->e     =getbitu2(buff,i+132,10,i+150,22)*P2_33;
    eph->cus   =getbits (buff,i+180,18)*P2_31;
    eph->crc   =getbits2(buff,i+198, 4,i+210,14)*P2_6;
    eph->crs   =getbits2(buff,i+224, 8,i+240,10)*P2_6;
    sqrtA      =getbitu2(buff,i+250,12,i+270,20)*P2_19;
    toe1       =getbitu (buff,i+290, 2); /* TOE 2-MSB */
    eph->A     =sqrtA*sqrtA;

    i=8*38*2; /* subframe 3 */
    frn3       =getbitu (buff,i+ 15, 3);
    sow3       =getbitu2(buff,i+ 18, 8,i+30,12);
    toe2       =getbitu2(buff,i+ 42,10,i+ 60, 5); /* TOE 5-LSB */
    eph->i0    =getbits2(buff,i+ 65,17,i+ 90,15)*P2_31*SC2RAD;
    eph->cic   =getbits2(buff,i+105, 7,i+120,11)*P2_31;
    eph->OMGd  =getbits2(buff,i+131,11,i+150,13)*P2_43*SC2RAD;
    eph->cis   =getbits2(buff,i+163, 9,i+180, 9)*P2_31;
    eph->idot  =getbits2(buff,i+189,13,i+210, 1)*P2_43*SC2RAD;
    eph->OMG0  =getbits2(buff,i+211,21,i+240,11)*P2_31*SC2RAD;
    eph->omg   =getbits2(buff,i+251,11,i+270,21)*P2_31*SC2RAD;
    eph->toes  =merge_two_u(toe1,toe2,15)*8.0;

    /* check consistency of subframe numbers, sows and toe/toc */
    if (frn1!=1||frn2!=2||frn3!=3) {
        printf("decode_bds_d1 error: frn=%d %d %d\n",frn1,frn2,frn3);
        return 0;
    }
    if (sow2!=sow1+6||sow3!=sow2+6) {
        printf("decode_bds_d1 error: sow=%d %d %d\n",sow1,sow2,sow3);
        return 0;
    }
    if (toc_bds!=eph->toes) {
        printf("decode_bds_d1 error: toe=%.0f toc=%.0f\n",eph->toes,toc_bds);
        return 0;
    }
    eph->ttr=bdt2gpst(bdt2time(eph->week,sow1));      /* bdt -> gpst */
    if      (eph->toes>sow1+302400.0) eph->week++;
    else if (eph->toes<sow1-302400.0) eph->week--;
    eph->toe=bdt2gpst(bdt2time(eph->week,eph->toes)); /* bdt -> gpst */
    eph->toc=bdt2gpst(bdt2time(eph->week,toc_bds));   /* bdt -> gpst */
    eph->code=0; /* data source = unknown */
    eph->flag=1; /* nav type = IGSO/MEO */
    return 1;
}

/* decode gps/qzss navigation data subframe 1 --------------------------------*/
static int decode_subfrm1(const unsigned char *buff, eph_t *eph)
{
    double tow,toc;
    int i=48,week,iodc0,iodc1,tgd;

    printf("decode_subfrm1:\n");
    // printf("decode_subfrm1: buff="); traceb(5,buff,30);

    tow        =getbitu(buff,24,17)*6.0;           /* transmission time */
    week       =getbitu(buff,i,10);       i+=10;
    eph->code  =getbitu(buff,i, 2);       i+= 2;
    eph->sva   =getbitu(buff,i, 4);       i+= 4;   /* ura index */
    eph->svh   =getbitu(buff,i, 6);       i+= 6;
    iodc0      =getbitu(buff,i, 2);       i+= 2;
    eph->flag  =getbitu(buff,i, 1);       i+= 1+87;
    tgd        =getbits(buff,i, 8);       i+= 8;
    iodc1      =getbitu(buff,i, 8);       i+= 8;
    toc        =getbitu(buff,i,16)*16.0;  i+=16;
    eph->f2    =getbits(buff,i, 8)*P2_55; i+= 8;
    eph->f1    =getbits(buff,i,16)*P2_43; i+=16;
    eph->f0    =getbits(buff,i,22)*P2_31;

    eph->tgd[0]=tgd==-128?0.0:tgd*P2_31; /* ref [4] */
    eph->iodc=(iodc0<<8)+iodc1;
    eph->week=adjgpsweek(week); /* week of tow */
    eph->ttr=gpst2time(eph->week,tow);
    eph->toc=gpst2time(eph->week,toc);

    return 1;
}
/* decode gps/qzss almanac ---------------------------------------------------*/
static void decode_almanac(const unsigned char *buff, int sat, alm_t *alm)
{
    gtime_t toa;
    double deltai,sqrtA,tt;
    int i=50,f0;

    printf("decode_almanac: sat=%2d\n",sat);

    if (!alm||alm[sat-1].week==0) return;

    alm[sat-1].sat =sat;
    alm[sat-1].e   =getbits(buff,i,16)*P2_21;        i+=16;
    alm[sat-1].toas=getbitu(buff,i, 8)*4096.0;       i+= 8;
    deltai         =getbits(buff,i,16)*P2_19*SC2RAD; i+=16;
    alm[sat-1].OMGd=getbits(buff,i,16)*P2_38*SC2RAD; i+=16;
    alm[sat-1].svh =getbitu(buff,i, 8);              i+= 8;
    sqrtA          =getbitu(buff,i,24)*P2_11;        i+=24;
    alm[sat-1].OMG0=getbits(buff,i,24)*P2_23*SC2RAD; i+=24;
    alm[sat-1].omg =getbits(buff,i,24)*P2_23*SC2RAD; i+=24;
    alm[sat-1].M0  =getbits(buff,i,24)*P2_23*SC2RAD; i+=24;
    f0             =getbitu(buff,i, 8);              i+= 8;
    alm[sat-1].f1  =getbits(buff,i,11)*P2_38;        i+=11;
    alm[sat-1].f0  =getbits(buff,i, 3)*P2_17+f0*P2_20;
    alm[sat-1].A   =sqrtA*sqrtA;
    alm[sat-1].i0  =0.3*SC2RAD+deltai;

    toa=gpst2time(alm[sat-1].week,alm[sat-1].toas);
    tt=timediff(toa,alm[sat-1].toa);
    if      (tt<302400.0) alm[sat-1].week--;
    else if (tt>302400.0) alm[sat-1].week++;
    alm[sat-1].toa=gpst2time(alm[sat-1].week,alm[sat-1].toas);
}
/* decode gps navigation data subframe 4 -------------------------------------*/
static void decode_gps_subfrm4(const unsigned char *buff, alm_t *alm,
                               double *ion, double *utc, int *leaps)
{
    int i,sat,svid=getbitu(buff,50,6);

    if (25<=svid&&svid<=32) { /* page 2,3,4,5,7,8,9,10 */

        /* decode almanac */
        sat=getbitu(buff,50,6);
        if (1<=sat&&sat<=32) decode_almanac(buff,sat,alm);
    }
    else if (svid==63) { /* page 25 */

        /* decode as and sv config */
        i=56;
        for (sat=1;sat<=32;sat++) {
            if (alm) alm[sat-1].svconf=getbitu(buff,i,4);
            i+=4;
        }
        /* decode sv health */
        i=186;
        for (sat=25;sat<=32;sat++) {
            if (alm) alm[sat-1].svh   =getbitu(buff,i,6);
            i+=6;
        }
    }
    else if (svid==56) { /* page 18 */

        /* decode ion/utc parameters */
        if (ion) {
            i=56;
            ion[0]=getbits(buff,i, 8)*P2_30;     i+= 8;
            ion[1]=getbits(buff,i, 8)*P2_27;     i+= 8;
            ion[2]=getbits(buff,i, 8)*P2_24;     i+= 8;
            ion[3]=getbits(buff,i, 8)*P2_24;     i+= 8;
            ion[4]=getbits(buff,i, 8)*pow(2,11); i+= 8;
            ion[5]=getbits(buff,i, 8)*pow(2,14); i+= 8;
            ion[6]=getbits(buff,i, 8)*pow(2,16); i+= 8;
            ion[7]=getbits(buff,i, 8)*pow(2,16);
        }
        if (utc) {
            i=120;
            utc[1]=getbits(buff,i,24)*P2_50;     i+=24;
            utc[0]=getbits(buff,i,32)*P2_30;     i+=32;
            utc[2]=getbits(buff,i, 8)*pow(2,12); i+= 8;
            utc[3]=getbitu(buff,i, 8);
        }
        if (leaps) {
            i=192;
            *leaps=getbits(buff,i,8);
        }
    }
}
/* decode gps navigation data subframe 5 -------------------------------------*/
static void decode_gps_subfrm5(const unsigned char *buff, alm_t *alm)
{
    double toas;
    int i,sat,week,svid=getbitu(buff,50,6);

    if (1<=svid&&svid<=24) { /* page 1-24 */

        /* decode almanac */
        sat=getbitu(buff,50,6);
        if (1<=sat&&sat<=32) decode_almanac(buff,sat,alm);
    }
    else if (svid==51) { /* page 25 */

        if (alm) {
            i=56;
            toas=getbitu(buff,i,8)*4096; i+=8;
            week=getbitu(buff,i,8);      i+=8;
            week=adjgpsweek(week);

            /* decode sv health */
            for (sat=1;sat<=24;sat++) {
                alm[sat-1].svh=getbitu(buff,i,6); i+=6;
            }
            for (sat=1;sat<=32;sat++) {
                alm[sat-1].toas=toas;
                alm[sat-1].week=week;
                alm[sat-1].toa=gpst2time(week,toas);
            }
        }
    }
}
/* decode qzss navigation data subframe 4/5 ----------------------------------*/
static void decode_qzs_subfrm45(const unsigned char *buff, alm_t *alm,
                                double *ion, double *utc, int *leaps)
{
    int i,j,sat,toas,week,svid=getbitu(buff,50,6);

    if (1<=svid&&svid<=5) { /* qzss almanac */

        if (!(sat=satno(SYS_QZS,192+svid))) return;
        decode_almanac(buff,sat,alm);
    }
    else if (svid==51) { /* qzss health */

        if (alm) {
            i=56;
            toas=getbitu(buff,i,8)*4096; i+=8;
            week=getbitu(buff,i,8);      i+=8;
            week=adjgpsweek(week);

            for (j=0;j<5;j++) {
                if (!(sat=satno(SYS_QZS,193+j))) continue;
                alm[sat-1].toas=toas;
                alm[sat-1].week=week;
                alm[sat-1].toa=gpst2time(week,toas);
                alm[sat-1].svh=getbitu(buff,i,6); i+=6;
            }
        }
    }
    else if (svid==56) { /* ion/utc parameters */

        if (ion) {
            i=56;
            ion[0]=getbits(buff,i, 8)*P2_30;     i+= 8;
            ion[1]=getbits(buff,i, 8)*P2_27;     i+= 8;
            ion[2]=getbits(buff,i, 8)*P2_24;     i+= 8;
            ion[3]=getbits(buff,i, 8)*P2_24;     i+= 8;
            ion[4]=getbits(buff,i, 8)*pow(2,11); i+= 8;
            ion[5]=getbits(buff,i, 8)*pow(2,14); i+= 8;
            ion[6]=getbits(buff,i, 8)*pow(2,16); i+= 8;
            ion[7]=getbits(buff,i, 8)*pow(2,16);
        }
        if (utc) {
            i=120;
            utc[1]=getbits(buff,i,24)*P2_50;     i+=24;
            utc[0]=getbits(buff,i,32)*P2_30;     i+=32;
            utc[2]=getbits(buff,i, 8)*pow(2,12); i+= 8;
            utc[3]=getbitu(buff,i, 8);
        }
    }
}

/* decode gps/qzss navigation data subframe 2 --------------------------------*/
static int decode_subfrm2(const unsigned char *buff, eph_t *eph)
{
    double sqrtA;
    int i=48;

    printf("decode_subfrm2:\n");
    // printf("decode_subfrm2: buff="); traceb(5,buff,30);

    eph->iode=getbitu(buff,i, 8);              i+= 8;
    eph->crs =getbits(buff,i,16)*P2_5;         i+=16;
    eph->deln=getbits(buff,i,16)*P2_43*SC2RAD; i+=16;
    eph->M0  =getbits(buff,i,32)*P2_31*SC2RAD; i+=32;
    eph->cuc =getbits(buff,i,16)*P2_29;        i+=16;
    eph->e   =getbitu(buff,i,32)*P2_33;        i+=32;
    eph->cus =getbits(buff,i,16)*P2_29;        i+=16;
    sqrtA    =getbitu(buff,i,32)*P2_19;        i+=32;
    eph->toes=getbitu(buff,i,16)*16.0;         i+=16;
    eph->fit =getbitu(buff,i, 1)?0.0:4.0; /* 0:4hr,1:>4hr */

    eph->A=sqrtA*sqrtA;

    return 2;
}

/* decode gps/qzss navigation data subframe 5 --------------------------------*/
static int decode_subfrm5(const unsigned char *buff, alm_t *alm, double *ion,
                          double *utc, int *leaps)
{
    int dataid=getbitu(buff,48,2);

    printf("decode_subfrm5: dataid=%d\n",dataid);
    //printf(5,"decode_subfrm5: buff="); traceb(5,buff,30);

    if (dataid==1) { /* gps */
        decode_gps_subfrm5(buff,alm);
    }
    else if (dataid==3) { /* qzss */
        decode_qzs_subfrm45(buff,alm,ion,utc,leaps);
    }
    return 5;
}
/* decode gps/qzss navigation data subframe 4 --------------------------------*/
static int decode_subfrm4(const unsigned char *buff, alm_t *alm, double *ion,
                          double *utc, int *leaps)
{
    int dataid=getbitu(buff,48,2);

    printf("decode_subfrm4: dataid=%d\n",dataid);
    // printf(5,"decode_subfrm4: buff="); traceb(5,buff,30);

    if (dataid==1) { /* gps */
        decode_gps_subfrm4(buff,alm,ion,utc,leaps);
    }
    else if (dataid==3) { /* qzss */
        decode_qzs_subfrm45(buff,alm,ion,utc,leaps);
    }
    return 4;
}
/* decode gps/qzss navigation data subframe 3 --------------------------------*/
static int decode_subfrm3(const unsigned char *buff, eph_t *eph)
{
    double tow,toc;
    int i=48,iode;

    printf("decode_subfrm3:\n");
    // printf("decode_subfrm3: buff="); traceb(5,buff,30);

    eph->cic =getbits(buff,i,16)*P2_29;        i+=16;
    eph->OMG0=getbits(buff,i,32)*P2_31*SC2RAD; i+=32;
    eph->cis =getbits(buff,i,16)*P2_29;        i+=16;
    eph->i0  =getbits(buff,i,32)*P2_31*SC2RAD; i+=32;
    eph->crc =getbits(buff,i,16)*P2_5;         i+=16;
    eph->omg =getbits(buff,i,32)*P2_31*SC2RAD; i+=32;
    eph->OMGd=getbits(buff,i,24)*P2_43*SC2RAD; i+=24;
    iode     =getbitu(buff,i, 8);              i+= 8;
    eph->idot=getbits(buff,i,14)*P2_43*SC2RAD;

    /* check iode and iodc consistency */
    if (iode!=eph->iode||iode!=(eph->iodc&0xFF)) return 0;

    /* adjustment for week handover */
    tow=time2gpst(eph->ttr,&eph->week);
    toc=time2gpst(eph->toc,NULL);
    if      (eph->toes<tow-302400.0) {eph->week++; tow-=604800.0;}
    else if (eph->toes>tow+302400.0) {eph->week--; tow+=604800.0;}
    eph->toe=gpst2time(eph->week,eph->toes);
    eph->toc=gpst2time(eph->week,toc);
    eph->ttr=gpst2time(eph->week,tow);

    return 3;
}

/* decode gps/qzss navigation data frame ---------------------------------------
* decode navigation data frame and extract ephemeris and ion/utc parameters
* args   : unsigned char *buff I gps navigation data frame (without parity)
*                                  buff[0-29]: 24 bits x 10 words
*          eph_t *eph    IO     ephemeris message      (NULL: no input)
*          alm_t *alm    IO     almanac                (NULL: no input)
*          double *ion   IO     ionospheric parameters (NULL: no input)
*          double *utc   IO     delta-utc parameters   (NULL: no input)
*          int   *leaps  IO     leap seconds (s)       (NULL: no input)
* return : status (0:no valid, 1-5:subframe id)
* notes  : use cpu time to resolve modulo 1024 ambiguity of the week number
*          see ref [1]
*          utc[3] reference week for utc parameter is truncated in 8 bits
*          ion and utc parameters by qzss indicate local iono and qzst-utc
*          parameters.
*-----------------------------------------------------------------------------*/
extern int decode_frame(const unsigned char *buff, eph_t *eph, alm_t *alm,
                        double *ion, double *utc, int *leaps)
{
    int id=getbitu(buff,43,3); /* subframe id */

    printf("decodefrm: id=%d\n",id);

    switch (id) {
        case 1: return decode_subfrm1(buff,eph);
        case 2: return decode_subfrm2(buff,eph);
        case 3: return decode_subfrm3(buff,eph);
        case 4: return decode_subfrm4(buff,alm,ion,utc,leaps);
        case 5: return decode_subfrm5(buff,alm,ion,utc,leaps);
    }
    return 0;
}

/* decode almanac ------------------------------------------------------------*/
static int decode_alm2(int sat, raw_t *raw)
{
  int sys=satsys(sat,NULL);

  printf("decode_alm2 : sat=%2d\n",sat);

  if (sys==SYS_GPS) {
      decode_frame(raw->subfrm[sat-1]+120,NULL,raw->nav.alm,NULL,NULL,NULL);
  }
  else if (sys==SYS_QZS) {
      decode_frame(raw->subfrm[sat-1]+120,NULL,raw->nav.alm,raw->nav.ion_qzs,
                   raw->nav.utc_qzs,&raw->nav.leaps);
      adj_utcweek(raw->time,raw->nav.utc_qzs);
  }
  return  0;
}
/* decode almanac and ion/utc ------------------------------------------------*/
static int decode_alm1(int sat, raw_t *raw)
{
  int sys=satsys(sat,NULL);

  printf("decode_alm1 : sat=%2d\n",sat);

  if (sys==SYS_GPS) {
      decode_frame(raw->subfrm[sat-1]+90,NULL,raw->nav.alm,raw->nav.ion_gps,
                   raw->nav.utc_gps,&raw->nav.leaps);
      adj_utcweek(raw->time,raw->nav.utc_gps);
  }
  else if (sys==SYS_QZS) {
      decode_frame(raw->subfrm[sat-1]+90,NULL,raw->nav.alm,raw->nav.ion_qzs,
                   raw->nav.utc_qzs,&raw->nav.leaps);
      adj_utcweek(raw->time,raw->nav.utc_qzs);
  }
  return 9;
}

/* decode ephemeris ----------------------------------------------------------*/
static int decode_ephem(int sat, raw_t *raw)
{
    eph_t eph={0};

    printf("decode_ephem: sat=%2d\n",sat);

    if (decode_frame(raw->subfrm[sat-1]   ,&eph,NULL,NULL,NULL,NULL)!=1||
        decode_frame(raw->subfrm[sat-1]+30,&eph,NULL,NULL,NULL,NULL)!=2||
        decode_frame(raw->subfrm[sat-1]+60,&eph,NULL,NULL,NULL,NULL)!=3) return 0;

    if (!strstr(raw->opt,"-EPHALL")) {
        if (eph.iode==raw->nav.eph[sat-1].iode&&
            eph.iodc==raw->nav.eph[sat-1].iodc) return 0; /* unchanged */
    }
    eph.sat=sat;
    raw->nav.eph[sat-1]=eph;
    raw->ephsat=sat;
    return 2;
}

/* decode sbas message ---------------------------------------------------------
* decode sbas message frame words and check crc
* args   : gtime_t time     I   reception time
*          int    prn       I   sbas satellite prn number
*          unsigned int *word I message frame words (24bit x 10)
*          sbsmsg_t *sbsmsg O   sbas message
* return : status (1:ok,0:crc error)
*-----------------------------------------------------------------------------*/
extern int sbsdecodemsg(gtime_t time, int prn, const unsigned int *words,
                        sbsmsg_t *sbsmsg)
{
    int i,j;
    unsigned char f[29];
    double tow;

    printf("sbsdecodemsg: prn=%d\n",prn);

    if (time.time==0) return 0;
    tow=time2gpst(time,&sbsmsg->week);
    sbsmsg->tow=(int)(tow+DTTOL);
    sbsmsg->prn=prn;
    for (i=0;i<7;i++) for (j=0;j<4;j++) {
        sbsmsg->msg[i*4+j]=(unsigned char)(words[i]>>((3-j)*8));
    }
    sbsmsg->msg[28]=(unsigned char)(words[7]>>18)&0xC0;
    for (i=28;i>0;i--) f[i]=(sbsmsg->msg[i]>>6)+(sbsmsg->msg[i-1]<<2);
    f[0]=sbsmsg->msg[0]>>6;

    return rtk_crc24q(f,29)==(words[7]&0xFFFFFF); /* check crc */
}

/* decode ubx-rxm-sfrb: subframe buffer --------------------------------------*/
static int decode_rxmsfrb(raw_t *raw)
{
  unsigned int words[10];
  int i,prn,sat,sys,id;
  unsigned char *p=raw->buff+6;

  printf("decode_rxmsfrb: len=%d\n",raw->len);

  if (raw->outtype) {
      sprintf(raw->msgtype,"UBX RXM-SFRB  (%4d): prn=%2d",raw->len,U1(p+1));
  }
  if (raw->len<42) {
      printf("ubx rxmsfrb length error: len=%d\n",raw->len);
      return -1;
  }
  prn=U1(p+1);
  if (!(sat=satno(MINPRNSBS<=prn?SYS_SBS:SYS_GPS,prn))) {
      printf("ubx rxmsfrb satellite number error: prn=%d\n",prn);
      return -1;
  }
  sys=satsys(sat,&prn);

  if (sys==SYS_GPS) {
      id=save_subfrm(sat,raw);
      if (id==3) return decode_ephem(sat,raw);
      if (id==4) return decode_alm1 (sat,raw);
      if (id==5) return decode_alm2 (sat,raw);
      return 0;
  }
  else if (sys==SYS_SBS) {
      for (i=0,p+=2;i<10;i++,p+=4) words[i]=U4(p);
      return sbsdecodemsg(raw->time,prn,words,&raw->sbsmsg)?3:0;
  }
  return 0;
}

/* decode gps and qzss navigation data ---------------------------------------*/
static int decode_nav(raw_t *raw, int sat, int off)
{
    unsigned int words[10];
    int i,id;
    unsigned char *p=raw->buff+6+off;

    if (raw->len<48+off) {
        printf("ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw->len);
        return -1;
    }
    if ((U4(p)>>24)==PREAMB_CNAV) {
        printf("ubx rawsfrbx cnav not supported sat=%d prn=%d\n",sat,
              (U4(p)>>18)&0x3F);
        return 0;
    }
    for (i=0;i<10;i++,p+=4) words[i]=U4(p)>>6; /* 24 bits without parity */

    id=(words[1]>>2)&7;
    if (id<1||5<id) {
        printf("ubx rawsfrbx subfrm id error: sat=%2d id=%d len=%d\n",sat,id,
              raw->len);
        return -1;
    }
    for (i=0;i<10;i++) {
        setbitu(raw->subfrm[sat-1]+(id-1)*30,i*24,24,words[i]);
    }
    if (id==3) return decode_ephem(sat,raw);
    if (id==4) return decode_alm1 (sat,raw);
    if (id==5) return decode_alm2 (sat,raw);
    return 0;
}


/* decode Galileo I/NAV ephemeris ----------------------------------------------
* decode Galileo I/NAV (ref [5] 4.3)
* args   : unsigned char *buff I Galileo I/NAV subframe bits
*                                  buff[ 0-15]: I/NAV word type 0 (128 bit)
*                                  buff[16-31]: I/NAV word type 1
*                                  buff[32-47]: I/NAV word type 2
*                                  buff[48-63]: I/NAV word type 3
*                                  buff[64-79]: I/NAV word type 4
*                                  buff[80-95]: I/NAV word type 5
*          eph_t    *eph    IO  ephemeris structure
* return : status (1:ok,0:error)
*-----------------------------------------------------------------------------*/
extern int decode_gal_inav(const unsigned char *buff, eph_t *eph)
{
    double tow,toc,tt,sqrtA;
    int i,time_f,week,svid,e5b_hs,e1b_hs,e5b_dvs,e1b_dvs,type[6],iod_nav[4];

    i=0; /* word type 0 */
    type[0]    =getbitu(buff,i, 6);              i+= 6;
    time_f     =getbitu(buff,i, 2);              i+= 2+88;
    week       =getbitu(buff,i,12);              i+=12; /* gst-week */
    tow        =getbitu(buff,i,20);

    i=128; /* word type 1 */
    type[1]    =getbitu(buff,i, 6);              i+= 6;
    iod_nav[0] =getbitu(buff,i,10);              i+=10;
    eph->toes  =getbitu(buff,i,14)*60.0;         i+=14;
    eph->M0    =getbits(buff,i,32)*P2_31*SC2RAD; i+=32;
    eph->e     =getbitu(buff,i,32)*P2_33;        i+=32;
    sqrtA      =getbitu(buff,i,32)*P2_19;

    i=128*2; /* word type 2 */
    type[2]    =getbitu(buff,i, 6);              i+= 6;
    iod_nav[1] =getbitu(buff,i,10);              i+=10;
    eph->OMG0  =getbits(buff,i,32)*P2_31*SC2RAD; i+=32;
    eph->i0    =getbits(buff,i,32)*P2_31*SC2RAD; i+=32;
    eph->omg   =getbits(buff,i,32)*P2_31*SC2RAD; i+=32;
    eph->idot  =getbits(buff,i,14)*P2_43*SC2RAD;

    i=128*3; /* word type 3 */
    type[3]    =getbitu(buff,i, 6);              i+= 6;
    iod_nav[2] =getbitu(buff,i,10);              i+=10;
    eph->OMGd  =getbits(buff,i,24)*P2_43*SC2RAD; i+=24;
    eph->deln  =getbits(buff,i,16)*P2_43*SC2RAD; i+=16;
    eph->cuc   =getbits(buff,i,16)*P2_29;        i+=16;
    eph->cus   =getbits(buff,i,16)*P2_29;        i+=16;
    eph->crc   =getbits(buff,i,16)*P2_5;         i+=16;
    eph->crs   =getbits(buff,i,16)*P2_5;         i+=16;
    eph->sva   =getbitu(buff,i, 8);

    i=128*4; /* word type 4 */
    type[4]    =getbitu(buff,i, 6);              i+= 6;
    iod_nav[3] =getbitu(buff,i,10);              i+=10;
    svid       =getbitu(buff,i, 6);              i+= 6;
    eph->cic   =getbits(buff,i,16)*P2_29;        i+=16;
    eph->cis   =getbits(buff,i,16)*P2_29;        i+=16;
    toc        =getbitu(buff,i,14)*60.0;         i+=14;
    eph->f0    =getbits(buff,i,31)*P2_34;        i+=31;
    eph->f1    =getbits(buff,i,21)*P2_46;        i+=21;
    eph->f2    =getbits(buff,i, 6)*P2_59;

    i=128*5; /* word type 5 */
    type[5]    =getbitu(buff,i, 6);              i+= 6+41;
    eph->tgd[0]=getbits(buff,i,10)*P2_32;        i+=10; /* BGD E5a/E1 */
    eph->tgd[1]=getbits(buff,i,10)*P2_32;        i+=10; /* BGD E5b/E1 */
    e5b_hs     =getbitu(buff,i, 2);              i+= 2;
    e1b_hs     =getbitu(buff,i, 2);              i+= 2;
    e5b_dvs    =getbitu(buff,i, 1);              i+= 1;
    e1b_dvs    =getbitu(buff,i, 1);

    /* test word types */
    if (type[0]!=0||type[1]!=1||type[2]!=2||type[3]!=3||type[4]!=4||type[5]!=5) {
        printf("decode_gal_inav error: type=%d %d %d %d %d %d\n",type[0],
              type[1],type[2],type[3],type[4],type[5]);
        return 0;
    }
    /* test word type 0 time field */
    if (time_f!=2) {
        printf("decode_gal_inav error: word0-time=%d\n",time_f);
        return 0;
    }
    /* test consistency of iod_nav */
    if (iod_nav[0]!=iod_nav[1]||iod_nav[0]!=iod_nav[2]||iod_nav[0]!=iod_nav[3]) {
        printf("decode_gal_inav error: ionav=%d %d %d %d\n",iod_nav[0],
              iod_nav[1],iod_nav[2],iod_nav[3]);
        return 0;
    }
    if (!(eph->sat=satno(SYS_GAL,svid))) {
        printf("decode_gal_inav svid error: svid=%d\n",svid);
        return 0;
    }
    eph->A=sqrtA*sqrtA;
    eph->iode=eph->iodc=iod_nav[0];
    eph->svh=(e5b_hs<<7)|(e5b_dvs<<6)|(e1b_hs<<1)|e1b_dvs;
    eph->ttr=gst2time(week,tow);
    tt=timediff(gst2time(week,eph->toes),eph->ttr); /* week complient to toe */
    if      (tt> 302400.0) week--;
    else if (tt<-302400.0) week++;
    eph->toe=gst2time(week,eph->toes);
    eph->toc=gst2time(week,toc);
    eph->week=week+1024; /* gal-week = gst-week + 1024 */
    eph->code =(1<<0)|(1<<9); /* data source = i/nav e1b, af0-2,toc,sisa for e5b-e1 */
    return 1;
}

/* decode galileo navigation data --------------------------------------------*/
static int decode_enav(raw_t *raw, int sat, int off)
{
    eph_t eph={0};
    unsigned char *p=raw->buff+6+off,buff[32],crc_buff[26]={0};
    int i,j,k,part1,page1,part2,page2,type;

    if (raw->len<44+off) {
        printf("ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw->len);
        return -1;
    }
    for (i=k=0;i<8;i++,p+=4) for (j=0;j<4;j++) {
        buff[k++]=p[3-j];
    }
    part1=getbitu(buff   ,0,1);
    page1=getbitu(buff   ,1,1);
    part2=getbitu(buff+16,0,1);
    page2=getbitu(buff+16,1,1);

    /* skip alert page */
    if (page1==1||page2==1) return 0;

    /* test even-odd parts */
    if (part1!=0||part2!=1) {
        printf("ubx rawsfrbx gal page even/odd error: sat=%2d\n",sat);
        return -1;
    }
    /* test crc (4(pad) + 114 + 82 bits) */
    for (i=0,j=  4;i<15;i++,j+=8) setbitu(crc_buff,j,8,getbitu(buff   ,i*8,8));
    for (i=0,j=118;i<11;i++,j+=8) setbitu(crc_buff,j,8,getbitu(buff+16,i*8,8));
    if (rtk_crc24q(crc_buff,25)!=getbitu(buff+16,82,24)) {
        printf("ubx rawsfrbx gal page crc error: sat=%2d\n",sat);
        return -1;
    }
    type=getbitu(buff,2,6); /* word type */

    /* skip word except for ephemeris, iono, utc parameters */
    if (type>6) return 0;

    /* clear word 0-6 flags */
    if (type==2) raw->subfrm[sat-1][112]=0;

    /* save page data (112 + 16 bits) to frame buffer */
    k=type*16;
    for (i=0,j=2;i<14;i++,j+=8) raw->subfrm[sat-1][k++]=getbitu(buff   ,j,8);
    for (i=0,j=2;i< 2;i++,j+=8) raw->subfrm[sat-1][k++]=getbitu(buff+16,j,8);

    /* test word 0-6 flags */
    raw->subfrm[sat-1][112]|=(1<<type);
    if (raw->subfrm[sat-1][112]!=0x7F) return 0;

    if (strstr(raw->opt,"-GALFNAV")) {
        return 0;
    }
    /* decode galileo inav ephemeris */
    if (!decode_gal_inav(raw->subfrm[sat-1],&eph)) {
        return 0;
    }
    /* test svid consistency */
    if (eph.sat!=sat) {
        printf("ubx rawsfrbx gal svid error: sat=%2d %2d\n",sat,eph.sat);
        return -1;
    }
    if (!strstr(raw->opt,"-EPHALL")) {
        if (eph.iode==raw->nav.eph[sat-1].iode&& /* unchanged */
            timediff(eph.toe,raw->nav.eph[sat-1].toe)==0.0&&
            timediff(eph.toc,raw->nav.eph[sat-1].toc)==0.0) return 0;
    }
    eph.sat=sat;
    raw->nav.eph[sat-1]=eph;
    raw->ephsat=sat;
    return 2;
}

static int merge_two_s(int a, unsigned int b, int n)
{
    return (int)((a<<n)+b);
}

/* decode BeiDou D2 ephemeris --------------------------------------------------
* decode BeiDou D2 ephemeris (GEO satellites) (ref [3] 5.3)
* args   : unsigned char *buff I beidou D2 subframe 1 page bits
*                                  buff[  0- 37]: page 1 (300 bits)
*                                  buff[ 38- 75]: page 2
*                                  ...
*                                  buff[342-379]: page 10
*          eph_t    *eph    IO  ephemeris structure
* return : status (1:ok,0:error)
*-----------------------------------------------------------------------------*/
extern int decode_bds_d2(const unsigned char *buff, eph_t *eph)
{
    double toc_bds,sqrtA;
    unsigned int f1p4,cucp5,ep6,cicp7,i0p8,OMGdp9,omgp10;
    unsigned int sow1,sow3,sow4,sow5,sow6,sow7,sow8,sow9,sow10;
    int i,f1p3,cucp4,ep5,cicp6,i0p7,OMGdp8,omgp9;
    int pgn1,pgn3,pgn4,pgn5,pgn6,pgn7,pgn8,pgn9,pgn10;

    printf("decode_bds_d2:\n");

    i=8*38*0; /* page 1 */
    pgn1       =getbitu (buff,i+ 42, 4);
    sow1       =getbitu2(buff,i+ 18, 8,i+ 30,12);
    eph->svh   =getbitu (buff,i+ 46, 1); /* SatH1 */
    eph->iodc  =getbitu (buff,i+ 47, 5); /* AODC */
    eph->sva   =getbitu (buff,i+ 60, 4);
    eph->week  =getbitu (buff,i+ 64,13); /* week in BDT */
    toc_bds    =getbitu2(buff,i+ 77, 5,i+ 90,12)*8.0;
    eph->tgd[0]=getbits (buff,i+102,10)*0.1*1E-9;
    eph->tgd[1]=getbits (buff,i+120,10)*0.1*1E-9;

    i=8*38*2; /* page 3 */
    pgn3       =getbitu (buff,i+ 42, 4);
    sow3       =getbitu2(buff,i+ 18, 8,i+ 30,12);
    eph->f0    =getbits2(buff,i+100,12,i+120,12)*P2_33;
    f1p3       =getbits (buff,i+132,4);

    i=8*38*3; /* page 4 */
    pgn4       =getbitu (buff,i+ 42, 4);
    sow4       =getbitu2(buff,i+ 18, 8,i+ 30,12);
    f1p4       =getbitu2(buff,i+ 46, 6,i+ 60,12);
    eph->f2    =getbits2(buff,i+ 72,10,i+ 90, 1)*P2_66;
    eph->iode  =getbitu (buff,i+ 91, 5); /* AODE */
    eph->deln  =getbits (buff,i+ 96,16)*P2_43*SC2RAD;
    cucp4      =getbits (buff,i+120,14);

    i=8*38*4; /* page 5 */
    pgn5       =getbitu (buff,i+ 42, 4);
    sow5       =getbitu2(buff,i+ 18, 8,i+ 30,12);
    cucp5      =getbitu (buff,i+ 46, 4);
    eph->M0    =getbits3(buff,i+ 50, 2,i+ 60,22,i+ 90, 8)*P2_31*SC2RAD;
    eph->cus   =getbits2(buff,i+ 98,14,i+120, 4)*P2_31;
    ep5        =getbits (buff,i+124,10);

    i=8*38*5; /* page 6 */
    pgn6       =getbitu (buff,i+ 42, 4);
    sow6       =getbitu2(buff,i+ 18, 8,i+ 30,12);
    ep6        =getbitu2(buff,i+ 46, 6,i+ 60,16);
    sqrtA      =getbitu3(buff,i+ 76, 6,i+ 90,22,i+120,4)*P2_19;
    cicp6      =getbits (buff,i+124,10);
    eph->A     =sqrtA*sqrtA;

    i=8*38*6; /* page 7 */
    pgn7       =getbitu (buff,i+ 42, 4);
    sow7       =getbitu2(buff,i+ 18, 8,i+ 30,12);
    cicp7      =getbitu2(buff,i+ 46, 6,i+ 60, 2);
    eph->cis   =getbits (buff,i+ 62,18)*P2_31;
    eph->toes  =getbitu2(buff,i+ 80, 2,i+ 90,15)*8.0;
    i0p7       =getbits2(buff,i+105, 7,i+120,14);

    i=8*38*7; /* page 8 */
    pgn8       =getbitu (buff,i+ 42, 4);
    sow8       =getbitu2(buff,i+ 18, 8,i+ 30,12);
    i0p8       =getbitu2(buff,i+ 46, 6,i+ 60, 5);
    eph->crc   =getbits2(buff,i+ 65,17,i+ 90, 1)*P2_6;
    eph->crs   =getbits (buff,i+ 91,18)*P2_6;
    OMGdp8     =getbits2(buff,i+109, 3,i+120,16);

    i=8*38*8; /* page 9 */
    pgn9       =getbitu (buff,i+ 42, 4);
    sow9       =getbitu2(buff,i+ 18, 8,i+ 30,12);
    OMGdp9     =getbitu (buff,i+ 46, 5);
    eph->OMG0  =getbits3(buff,i+ 51, 1,i+ 60,22,i+ 90, 9)*P2_31*SC2RAD;
    omgp9      =getbits2(buff,i+ 99,13,i+120,14);

    i=8*38*9; /* page 10 */
    pgn10      =getbitu (buff,i+ 42, 4);
    sow10      =getbitu2(buff,i+ 18, 8,i+ 30,12);
    omgp10     =getbitu (buff,i+ 46, 5);
    eph->idot  =getbits2(buff,i+ 51, 1,i+ 60,13)*P2_43*SC2RAD;

    /* check consistency of page numbers, sows and toe/toc */
    if (pgn1!=1||pgn3!=3||pgn4!=4||pgn5!=5||pgn6!=6||pgn7!=7||pgn8!=8||pgn9!=9||
        pgn10!=10) {
        printf("decode_bds_d2 error: pgn=%d %d %d %d %d %d %d %d %d\n",
              pgn1,pgn3,pgn4,pgn5,pgn6,pgn7,pgn8,pgn9,pgn10);
        return 0;
    }
    if (sow3!=sow1+6||sow4!=sow3+3||sow5!=sow4+3||sow6!=sow5+3||
        sow7!=sow6+3||sow8!=sow7+3||sow9!=sow8+3||sow10!=sow9+3) {
        printf("decode_bds_d2 error: sow=%d %d %d %d %d %d %d %d %d\n",
              sow1,sow3,sow4,sow5,sow6,sow7,sow8,sow9,sow10);
        return 0;
    }
    if (toc_bds!=eph->toes) {
        printf("decode_bds_d2 error: toe=%.0f toc=%.0f\n",eph->toes,toc_bds);
        return 0;
    }
    eph->f1  =merge_two_s(f1p3  ,f1p4  ,18)*P2_50;
    eph->cuc =merge_two_s(cucp4 ,cucp5 , 4)*P2_31;
    eph->e   =merge_two_s(ep5   ,ep6   ,22)*P2_33;
    eph->cic =merge_two_s(cicp6 ,cicp7 , 8)*P2_31;
    eph->i0  =merge_two_s(i0p7  ,i0p8  ,11)*P2_31*SC2RAD;
    eph->OMGd=merge_two_s(OMGdp8,OMGdp9, 5)*P2_43*SC2RAD;
    eph->omg =merge_two_s(omgp9 ,omgp10, 5)*P2_31*SC2RAD;

    eph->ttr=bdt2gpst(bdt2time(eph->week,sow1));      /* bdt -> gpst */
    if      (eph->toes>sow1+302400.0) eph->week++;
    else if (eph->toes<sow1-302400.0) eph->week--;
    eph->toe=bdt2gpst(bdt2time(eph->week,eph->toes)); /* bdt -> gpst */
    eph->toc=bdt2gpst(bdt2time(eph->week,toc_bds));   /* bdt -> gpst */
    eph->code=0; /* data source = unknown */
    eph->flag=2; /* nav type = GEO */
    return 1;
}

/* decode beidou navigation data ---------------------------------------------*/
static int decode_cnav(raw_t *raw, int sat, int off)
{
    eph_t eph={0};
    unsigned int words[10];
    int i,id,pgn,prn;
    unsigned char *p=raw->buff+6+off;

    if (raw->len<48+off) {
        printf("ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw->len);
        return -1;
    }
    for (i=0;i<10;i++,p+=4) words[i]=U4(p)&0x3FFFFFFF; /* 30 bits */

    satsys(sat,&prn);
    id=(words[0]>>12)&0x07; /* subframe id (3bit) */
    if (id<1||5<id) {
        printf("ubx rawsfrbx subfrm id error: sat=%2d\n",sat);
        return -1;
    }
    if (prn>5&&prn<59) { /* IGSO/MEO */

        for (i=0;i<10;i++) {
            setbitu(raw->subfrm[sat-1]+(id-1)*38,i*30,30,words[i]);
        }
        if (id!=3) return 0;

        /* decode beidou D1 ephemeris */
        if (!decode_bds_d1(raw->subfrm[sat-1],&eph)) return 0;
    }
    else { /* GEO (C01-05, C59-63) */
        if (id!=1) return 0;

        /* subframe 1 */
        pgn=(words[1]>>14)&0x0F; /* page number (4bit) */
        if (pgn<1||10<pgn) {
            printf("ubx rawsfrbx page number error: sat=%2d\n",sat);
            return -1;
        }
        for (i=0;i<10;i++) {
            setbitu(raw->subfrm[sat-1]+(pgn-1)*38,i*30,30,words[i]);
        }
        if (pgn!=10) return 0;

        /* decode beidou D2 ephemeris */
        if (!decode_bds_d2(raw->subfrm[sat-1],&eph)) return 0;
    }
    if (!strstr(raw->opt,"-EPHALL")) {
        if (timediff(eph.toe,raw->nav.eph[sat-1].toe)==0.0&&
            eph.iode==raw->nav.eph[sat-1].iode&&
            eph.iodc==raw->nav.eph[sat-1].iodc) return 0; /* unchanged */
    }
    eph.sat=sat;
    raw->nav.eph[sat-1]=eph;
    raw->ephsat=sat;
    return 2;
}
/* get sign-magnitude bits ---------------------------------------------------*/
static double getbitg(const unsigned char *buff, int pos, int len)
{
    double value=getbitu(buff,pos+1,len-1);
    return getbitu(buff,pos,1)?-value:value;
}
/* decode glonass ephemeris strings --------------------------------------------
* decode glonass ephemeris string (ref [2])
* args   : unsigned char *buff I glonass navigation data string bits in frames
*                                (without hamming and time mark)
*                                  buff[ 0- 9]: string #1 (77 bits)
*                                  buff[10-19]: string #2
*                                  buff[20-29]: string #3
*                                  buff[30-39]: string #4
*          geph_t *geph  IO     glonass ephemeris message
* return : status (1:ok,0:error)
* notes  : geph->tof should be set to frame time witin 1/2 day before calling
*          geph->frq is set to 0
*-----------------------------------------------------------------------------*/
extern int decode_glostr(const unsigned char *buff, geph_t *geph)
{
    double tow,tod,tof,toe;
    int P,P1,P2,P3,P4,tk_h,tk_m,tk_s,tb,ln,NT,slot,M,week;
    int i=1,frn1,frn2,frn3,frn4;

    printf("decode_glostr:\n");

    /* frame 1 */
    frn1        =getbitu(buff,i, 4);           i+= 4+2;
    P1          =getbitu(buff,i, 2);           i+= 2;
    tk_h        =getbitu(buff,i, 5);           i+= 5;
    tk_m        =getbitu(buff,i, 6);           i+= 6;
    tk_s        =getbitu(buff,i, 1)*30;        i+= 1;
    geph->vel[0]=getbitg(buff,i,24)*P2_20*1E3; i+=24;
    geph->acc[0]=getbitg(buff,i, 5)*P2_30*1E3; i+= 5;
    geph->pos[0]=getbitg(buff,i,27)*P2_11*1E3; i+=27+4;

    /* frame 2 */
    frn2        =getbitu(buff,i, 4);           i+= 4;
    geph->svh   =getbitu(buff,i, 3);           i+= 3;
    P2          =getbitu(buff,i, 1);           i+= 1;
    tb          =getbitu(buff,i, 7);           i+= 7+5;
    geph->vel[1]=getbitg(buff,i,24)*P2_20*1E3; i+=24;
    geph->acc[1]=getbitg(buff,i, 5)*P2_30*1E3; i+= 5;
    geph->pos[1]=getbitg(buff,i,27)*P2_11*1E3; i+=27+4;

    /* frame 3 */
    frn3        =getbitu(buff,i, 4);           i+= 4;
    P3          =getbitu(buff,i, 1);           i+= 1;
    geph->gamn  =getbitg(buff,i,11)*P2_40;     i+=11+1;
    P           =getbitu(buff,i, 2);           i+= 2;
    ln          =getbitu(buff,i, 1);           i+= 1;
    geph->vel[2]=getbitg(buff,i,24)*P2_20*1E3; i+=24;
    geph->acc[2]=getbitg(buff,i, 5)*P2_30*1E3; i+= 5;
    geph->pos[2]=getbitg(buff,i,27)*P2_11*1E3; i+=27+4;

    /* frame 4 */
    frn4        =getbitu(buff,i, 4);           i+= 4;
    geph->taun  =getbitg(buff,i,22)*P2_30;     i+=22;
    geph->dtaun =getbitg(buff,i, 5)*P2_30;     i+= 5;
    geph->age   =getbitu(buff,i, 5);           i+= 5+14;
    P4          =getbitu(buff,i, 1);           i+= 1;
    geph->sva   =getbitu(buff,i, 4);           i+= 4+3;
    NT          =getbitu(buff,i,11);           i+=11;
    slot        =getbitu(buff,i, 5);           i+= 5;
    M           =getbitu(buff,i, 2);

    if (frn1!=1||frn2!=2||frn3!=3||frn4!=4) {
        printf("decode_glostr error: frn=%d %d %d %d %d\n",frn1,frn2,frn3,frn4);
        return 0;
    }
    if (!(geph->sat=satno(SYS_GLO,slot))) {
        printf("decode_glostr error: slot=%d\n",slot);
        return 0;
    }
    geph->frq=0;
    geph->iode=tb;
    tow=time2gpst(gpst2utc(geph->tof),&week);
    tod=fmod(tow,86400.0); tow-=tod;
    tof=tk_h*3600.0+tk_m*60.0+tk_s-10800.0; /* lt->utc */
    if      (tof<tod-43200.0) tof+=86400.0;
    else if (tof>tod+43200.0) tof-=86400.0;
    geph->tof=utc2gpst(gpst2time(week,tow+tof));
    toe=tb*900.0-10800.0; /* lt->utc */
    if      (toe<tod-43200.0) toe+=86400.0;
    else if (toe>tod+43200.0) toe-=86400.0;
    geph->toe=utc2gpst(gpst2time(week,tow+toe)); /* utc->gpst */
    return 1;
}
/* decode glonass navigation data --------------------------------------------*/
static int decode_gnav(raw_t *raw, int sat, int off, int frq)
{
    geph_t geph={0};
    int i,j,k,m,prn;
    unsigned char *p=raw->buff+6+off,buff[64],*fid;

    satsys(sat,&prn);

    if (raw->len<24+off) {
        printf("ubx rawsfrbx gnav length error: len=%d\n",raw->len);
        return -1;
    }
    for (i=k=0;i<4;i++,p+=4) for (j=0;j<4;j++) {
        buff[k++]=p[3-j];
    }
    /* test hamming of glonass string */
    if (!test_glostr(buff)) {
        printf("ubx rawsfrbx glo string hamming error: sat=%2d\n",sat);
        return -1;
    }
    m=getbitu(buff,1,4);
    if (m<1||15<m) {
        printf("ubx rawsfrbx glo string no error: sat=%2d\n",sat);
        return -1;
    }
    /* flush frame buffer if frame-id changed */
    fid=raw->subfrm[sat-1]+150;
    if (fid[0]!=buff[12]||fid[1]!=buff[13]) {
        for (i=0;i<4;i++) memset(raw->subfrm[sat-1]+i*10,0,10);
        memcpy(fid,buff+12,2); /* save frame-id */
    }
    memcpy(raw->subfrm[sat-1]+(m-1)*10,buff,10);

    if (m!=4) return 0;

    /* decode glonass ephemeris strings */
    geph.tof=raw->time;
    if (!decode_glostr(raw->subfrm[sat-1],&geph)||geph.sat!=sat) return 0;
    geph.frq=frq-7;

    if (!strstr(raw->opt,"-EPHALL")) {
        if (geph.iode==raw->nav.geph[prn-1].iode) return 0; /* unchanged */
    }
    raw->nav.geph[prn-1]=geph;
    raw->ephsat=sat;
    return 2;
}
/* decode sbas navigation data -----------------------------------------------*/
static int decode_snav(raw_t *raw, int sat, int off)
{
    int i,j,k,prn,tow,week;
    unsigned char *p=raw->buff+6+off,buff[64];

    if (raw->len<40+off) {
        printf("ubx rawsfrbx snav length error: len=%d\n",raw->len);
        return -1;
    }
    tow=(int)time2gpst(timeadd(raw->time,-1.0),&week);
    satsys(sat,&prn);
    raw->sbsmsg.prn=prn;
    raw->sbsmsg.tow=tow;
    raw->sbsmsg.week=week;
    for (i=k=0;i<8;i++,p+=4) for (j=0;j<4;j++) {
        buff[k++]=p[3-j];
    }
    memcpy(raw->sbsmsg.msg,buff,29);
    raw->sbsmsg.msg[28]&=0xC0;
    return 3;
}
/* decode ubx-rxm-sfrbx: raw subframe data (ref [3][4][5]) -------------------*/
static int decode_rxmsfrbx(raw_t *raw)
{
    int prn,sat,sys;
    unsigned char *p=raw->buff+6;

    (4,"decode_rxmsfrbx: len=%d\n",raw->len);

    if (raw->outtype) {
        sprintf(raw->msgtype,"UBX RXM-SFRBX (%4d): sys=%d prn=%3d",raw->len,
                U1(p),U1(p+1));
    }
    if (!(sys=ubx_sys(U1(p)))) {
        printf("ubx rxmsfrbx sys id error: sys=%d\n",U1(p));
        return -1;
    }
    prn=U1(p+1)+(sys==SYS_QZS?192:0);
    if (!(sat=satno(sys,prn))) {
        if (sys==SYS_GLO&&prn==255) {
            return 0; /* suppress error for unknown glo satellite */
        }
        printf("ubx rxmsfrbx sat number error: sys=%d prn=%d\n",sys,prn);
        return -1;
    }
    switch (sys) {
        case SYS_GPS: return decode_nav (raw,sat,8);
        case SYS_QZS: return decode_nav (raw,sat,8);
        case SYS_GAL: return decode_enav(raw,sat,8);
        case SYS_CMP: return decode_cnav(raw,sat,8);
        case SYS_GLO: return decode_gnav(raw,sat,8,U1(p+3));
        case SYS_SBS: return decode_snav(raw,sat,8);
    }
    return 0;
}
