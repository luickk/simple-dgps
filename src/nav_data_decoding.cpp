#include "gps_timing.cpp"
#include "gps_sat.cpp"
#include "ubx_parsing.cpp"

/* contains functions for decoding of ubx subframes to extract data such as ephemeris, position data */

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


/* save subframe -------------------------------------------------------------*/
static int save_subfrm(int sat, raw_t *raw)
{
    unsigned char *p=raw->buff+6,*q;
    int i,j,n,id=(U4(p+6)>>2)&0x7;

    #ifdef LOG_DECODING_MSGS
    printf("save_subfrm: sat=%2d id=%d\n",sat,id);
    #endif

    if (id<1||5<id) return 0;

    q=raw->subfrm[sat-1]+(id-1)*30;

    for (i=n=0,p+=2;i<10;i++,p+=4) {
        for (j=23;j>=0;j--) {
            *q=(*q<<1)+((U4(p)>>j)&1); if (++n%8==0) q++;
        }
    }
    return id;
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

/* ubx sigid to signal ([5] Appendix B) --------------------------------------*/
static int ubx_sig(int sys, int sigid)
{
    if (sys == SYS_GPS) {
        if (sigid == 0) return CODE_L1C; /* L1C/A */
        if (sigid == 3) return CODE_L2L; /* L2CL */
        if (sigid == 4) return CODE_L2M; /* L2CM */
    }
    else if (sys == SYS_GLO) {
        if (sigid == 0) return CODE_L1C; /* G1C/A (GLO L1 OF) */
        if (sigid == 2) return CODE_L2C; /* G2C/A (GLO L2 OF) */
    }
    else if (sys == SYS_GAL) {
        if (sigid == 0) return CODE_L1C; /* E1C */
        if (sigid == 1) return CODE_L1B; /* E1B */
        if (sigid == 5) return CODE_L7I; /* E5bI */
        if (sigid == 6) return CODE_L7Q; /* E5bQ */
    }
    else if (sys == SYS_QZS) {
        if (sigid == 0) return CODE_L1C; /* L1C/A */
        if (sigid == 5) return CODE_L2L; /* L2CL (not specified in [5]) */
    }
    else if (sys == SYS_CMP) {
        if (sigid == 0) return CODE_L1I; /* B1I D1 (rinex 3.02) */
        if (sigid == 1) return CODE_L1I; /* B1I D2 (rinex 3.02) */
        if (sigid == 2) return CODE_L7I; /* B2I D1 */
        if (sigid == 3) return CODE_L7I; /* B2I D2 */
    }
    else if (sys == SYS_SBS) {
        return CODE_L1C; /* L1C/A (not in [5]) */
    }
    return CODE_NONE;
}
/* signal index in obs data --------------------------------------------------*/
static int sig_idx(int sys, int code)
{
    if (sys == SYS_GPS) {
        if (code==CODE_L1C) return 1;
        if (code==CODE_L2L) return 2;
        if (code==CODE_L2M) return NFREQ+1;
    }
    else if (sys == SYS_GLO) {
        if (code==CODE_L1C) return 1;
        if (code==CODE_L2C) return 2;
    }
    else if (sys == SYS_GAL) {
        if (code==CODE_L1C) return 1;
        if (code==CODE_L1B) return NFREQ+1;
        if (code==CODE_L7I) return 2; /* E5bI */
        if (code==CODE_L7Q) return 2; /* E5bQ */
    }
    else if (sys == SYS_QZS) {
        if (code==CODE_L1C) return 1;
        if (code==CODE_L2L) return 2;
    }
    else if (sys == SYS_CMP) {
        if (code==CODE_L1I) return 1;
        if (code==CODE_L7I) return 2;
    }
    else if (sys == SYS_SBS) {
        if (code==CODE_L1C) return 1;
    }
    return 0;
}
/* freq index to frequency ---------------------------------------------------*/
static double sig_freq(int sys, int f, int fcn)
{
    static const double freq_glo[8]={FREQ1_GLO,FREQ2_GLO,FREQ3_GLO};
    static const double dfrq_glo[8]={DFRQ1_GLO,DFRQ2_GLO};
    static const double freq_bds[8]={FREQ1_CMP,0,0,FREQ3_CMP,FREQ2_CMP};

    if (sys == SYS_GLO) {
        return freq_glo[f-1]+dfrq_glo[f-1]*fcn;
    }
    else if (sys == SYS_CMP) {
        return freq_bds[f-1];
    }
    return CLIGHT/lam_carr[f-1];
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

/* free receiver raw data control ----------------------------------------------
* free observation and ephemeris buffer in receiver raw data control struct
* args   : raw_t  *raw   IO     receiver raw data control struct
* return : none
*-----------------------------------------------------------------------------*/
extern void free_raw(raw_t *raw)
{
    printf("free_raw:\n");

    free(raw->obs.data ); raw->obs.data =NULL; raw->obs.n =0;
    free(raw->obuf.data); raw->obuf.data=NULL; raw->obuf.n=0;
    free(raw->nav.eph  ); raw->nav.eph  =NULL; raw->nav.n =0;
    free(raw->nav.alm  ); raw->nav.alm  =NULL; raw->nav.na=0;
    free(raw->nav.geph ); raw->nav.geph =NULL; raw->nav.ng=0;
    free(raw->nav.seph ); raw->nav.seph =NULL; raw->nav.ns=0;
}

/* initialize receiver raw data control ----------------------------------------
* initialize receiver raw data control struct and reallocate obsevation and
* epheris buffer
* args   : raw_t  *raw   IO     receiver raw data control struct
* return : status (1:ok,0:memory allocation error)
*-----------------------------------------------------------------------------*/
extern int init_raw(raw_t *raw)
{
    const double lam_glo[NFREQ]={CLIGHT/FREQ1_GLO,CLIGHT/FREQ2_GLO};
    gtime_t time0={0};
    obsd_t data0={{0}};
    eph_t  eph0 ={0,-1,-1};
    alm_t  alm0 ={0,-1};
    geph_t geph0={0,-1};
    seph_t seph0={0};
    sbsmsg_t sbsmsg0={0};
    lexmsg_t lexmsg0={0};
    int i,j,sys,ret=1;


    raw->time=time0;
    raw->ephsat=0;
    raw->sbsmsg=sbsmsg0;
    raw->msgtype[0]='\0';
    for (i=0;i<MAXSAT;i++) {
        for (j=0;j<380;j++) raw->subfrm[i][j]=0;
        for (j=0;j<NFREQ+NEXOBS;j++) {
            raw->tobs [i][j]=time0;
            raw->lockt[i][j]=0.0;
            raw->halfc[i][j]=0;
        }
        raw->icpp[i]=raw->off[i]=raw->prCA[i]=raw->dpCA[i]=0.0;
    }
    for (i=0;i<MAXOBS;i++) raw->freqn[i]=0;
    raw->lexmsg=lexmsg0;
    raw->icpc=0.0;
    raw->nbyte=raw->len=0;
    raw->iod=raw->flag=raw->tbase=raw->outtype=0;
    raw->tod=-1;
    for (i=0;i<MAXRAWLEN;i++) raw->buff[i]=0;
    raw->opt[0]='\0';

    raw->obs.data =NULL;
    raw->obuf.data=NULL;
    raw->nav.eph  =NULL;
    raw->nav.alm  =NULL;
    raw->nav.geph =NULL;
    raw->nav.seph =NULL;

    if (!(raw->obs.data =(obsd_t *)malloc(sizeof(obsd_t)*MAXOBS))||
        !(raw->obuf.data=(obsd_t *)malloc(sizeof(obsd_t)*MAXOBS))||
        !(raw->nav.eph  =(eph_t  *)malloc(sizeof(eph_t )*MAXSAT))||
        !(raw->nav.alm  =(alm_t  *)malloc(sizeof(alm_t )*MAXSAT))||
        !(raw->nav.geph =(geph_t *)malloc(sizeof(geph_t)*NSATGLO))||
        !(raw->nav.seph =(seph_t *)malloc(sizeof(seph_t)*NSATSBS*2))) {
        free_raw(raw);
        return 0;
    }
    raw->obs.n =0;
    raw->obuf.n=0;
    raw->nav.n =MAXSAT;
    raw->nav.na=MAXSAT;
    raw->nav.ng=NSATGLO;
    raw->nav.ns=NSATSBS*2;
    for (i=0;i<MAXOBS   ;i++) raw->obs.data [i]=data0;
    for (i=0;i<MAXOBS   ;i++) raw->obuf.data[i]=data0;
    for (i=0;i<MAXSAT   ;i++) raw->nav.eph  [i]=eph0;
    for (i=0;i<MAXSAT   ;i++) raw->nav.alm  [i]=alm0;
    for (i=0;i<NSATGLO  ;i++) raw->nav.geph [i]=geph0;
    for (i=0;i<NSATSBS*2;i++) raw->nav.seph [i]=seph0;
    for (i=0;i<MAXSAT;i++) for (j=0;j<NFREQ;j++) {
        if (!(sys=satsys(i+1,NULL))) continue;
        raw->nav.lam[i][j]=sys==SYS_GLO?lam_glo[j]:lam_carr[j];
    }
    raw->sta.name[0]=raw->sta.marker[0]='\0';
    raw->sta.antdes[0]=raw->sta.antsno[0]='\0';
    raw->sta.rectype[0]=raw->sta.recver[0]=raw->sta.recsno[0]='\0';
    raw->sta.antsetup=raw->sta.itrf=raw->sta.deltype=0;
    for (i=0;i<3;i++) {
        raw->sta.pos[i]=raw->sta.del[i]=0.0;
    }
    raw->sta.hgt=0.0;

    if (!ret) {
        free_raw(raw);
        return 0;
    }
    return 1;
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

    #ifdef LOG_DECODING_MSGS
    printf("decode_bds_d1:\n");
    #endif

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
        #ifdef LOG_DECODING_MSGS
        printf("decode_bds_d1 error: frn=%d %d %d\n",frn1,frn2,frn3);
        #endif
        return 0;
    }
    if (sow2!=sow1+6||sow3!=sow2+6) {
        #ifdef LOG_DECODING_MSGS
        printf("decode_bds_d1 error: sow=%d %d %d\n",sow1,sow2,sow3);
        #endif
        return 0;
    }
    if (toc_bds!=eph->toes) {
        #ifdef LOG_DECODING_MSGS
        printf("decode_bds_d1 error: toe=%.0f toc=%.0f\n",eph->toes,toc_bds);
        #endif
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

    #ifdef LOG_DECODING_MSGS
    printf("decode_subfrm1:\n");
    #endif
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

    #ifdef LOG_DECODING_MSGS
    printf("decode_almanac: sat=%2d\n",sat);
    #endif

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

    #ifdef LOG_DECODING_MSGS
    printf("decode_subfrm2:\n");
    #endif
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

    #ifdef LOG_DECODING_MSGS
    printf("decode_subfrm5: dataid=%d\n",dataid);
    #endif
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
    #ifdef LOG_DECODING_MSGS
    printf("decode_subfrm4: dataid=%d\n",dataid);
    #endif
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

    #ifdef LOG_DECODING_MSGS
    printf("decode_subfrm3:\n");
    #endif
    // printf("decode_subfrm3: buff= %c", buff);

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
    #ifdef LOG_DECODING_MSGS
    printf("decodefrm: id=%d\n",id);
    #endif
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
  #ifdef LOG_DECODING_MSGS
  printf("decode_alm2 : sat=%2d\n",sat);
  #endif
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
  #ifdef LOG_DECODING_MSGS
  printf("decode_alm1 : sat=%2d\n",sat);
  #endif
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
static int decode_ephem(int sat, raw_t *raw, std::vector<sat_pos> *satellites_array)
{
    eph_t eph={0};
    #ifdef LOG_DECODING_MSGS
    printf("decode_ephem: sat=% 2d \n",sat);
    #endif
    if (decode_frame(raw->subfrm[sat-1]   ,&eph,NULL,NULL,NULL,NULL)!=1||
        decode_frame(raw->subfrm[sat-1]+30,&eph,NULL,NULL,NULL,NULL)!=2||
        decode_frame(raw->subfrm[sat-1]+60,&eph,NULL,NULL,NULL,NULL)!=3) return 0;

    for(int i=0; i<satellites_array->size(); i++)
    {
        if((*satellites_array)[i].satno==sat)
        {
          (*satellites_array)[i].eph = eph;
        }
    }

    eph.sat=sat;
    raw->nav.eph[sat-1]=eph;
    raw->ephsat=sat;
    return 2;
}

/* decode ubx-rxm-sfrb: subframe buffer --------------------------------------*/
static int decode_rxmsfrb(raw_t *raw, std::vector<sat_pos> *satellites_array)
{
  unsigned int words[10];
  int i,prn,sat,sys,id;
  unsigned char *p=raw->buff+6;
  #ifdef LOG_DECODING_MSGS
  printf("decode_rxmsfrb: len=%d\n",raw->len);
  #endif
  if (raw->outtype) {
      sprintf(raw->msgtype,"UBX RXM-SFRB  (%4d): prn=%2d",raw->len,U1(p+1));
  }
  if (raw->len<42) {
      #ifdef LOG_DECODING_MSGS
      printf("ubx rxmsfrb length error: len=%d\n",raw->len);
      #endif
      return -1;
  }
  prn=U1(p+1);
  if (!(sat=satno(MINPRNSBS<=prn?SYS_SBS:SYS_GPS,prn))) {
      #ifdef LOG_DECODING_MSGS
      printf("ubx rxmsfrb satellite number error: prn=%d\n",prn);
      #endif
      return -1;
  }
  sys=satsys(sat,&prn);

  if (sys==SYS_GPS) {
      id=save_subfrm(sat,raw);
      if (id==3) return decode_ephem(sat,raw, satellites_array);
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
static int decode_nav(raw_t *raw, int sat, int off, std::vector<sat_pos> *satellites_array)
{
    unsigned int words[10];
    int i,id;
    unsigned char *p=raw->buff+6+off;

    if (raw->len<48+off) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw->len);
        #endif
        return -1;
    }
    if ((U4(p)>>24)==PREAMB_CNAV) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx cnav not supported sat=%d prn=%d\n",sat,
              (U4(p)>>18)&0x3F);
        #endif
        return 0;
    }
    for (i=0;i<10;i++,p+=4) words[i]=U4(p)>>6; /* 24 bits without parity */

    id=(words[1]>>2)&7;
    if (id<1||5<id) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx subfrm id error: sat=%2d id=%d len=%d\n",sat,id,
              raw->len);
        #endif
        return -1;
    }
    for (i=0;i<10;i++) {
        setbitu(raw->subfrm[sat-1]+(id-1)*30,i*24,24,words[i]);
    }
    if (id==3) return decode_ephem(sat,raw, satellites_array);
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
        #ifdef LOG_DECODING_MSGS
        printf("decode_gal_inav error: type=%d %d %d %d %d %d\n",type[0],
              type[1],type[2],type[3],type[4],type[5]);
        #endif
        return 0;
    }
    /* test word type 0 time field */
    if (time_f!=2) {
        #ifdef LOG_DECODING_MSGS
        printf("decode_gal_inav error: word0-time=%d\n",time_f);
        #endif
        return 0;
    }
    /* test consistency of iod_nav */
    if (iod_nav[0]!=iod_nav[1]||iod_nav[0]!=iod_nav[2]||iod_nav[0]!=iod_nav[3]) {
        #ifdef LOG_DECODING_MSGS
        printf("decode_gal_inav error: ionav=%d %d %d %d\n",iod_nav[0],
              iod_nav[1],iod_nav[2],iod_nav[3]);
        #endif
        return 0;
    }
    if (!(eph->sat=satno(SYS_GAL,svid))) {
        #ifdef LOG_DECODING_MSGS
        printf("decode_gal_inav svid error: svid=%d\n",svid);
        #endif
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
static int decode_enav(raw_t *raw, int sat, int off, std::vector<sat_pos> *satellites_array)
{
    eph_t eph={0};
    unsigned char *p=raw->buff+6+off,buff[32],crc_buff[26]={0};
    int i,j,k,part1,page1,part2,page2,type;

    if (raw->len<44+off) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw->len);
        #endif
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
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx gal page even/odd error: sat=%2d\n",sat);
        #endif
        return -1;
    }
    /* test crc (4(pad) + 114 + 82 bits) */
    for (i=0,j=  4;i<15;i++,j+=8) setbitu(crc_buff,j,8,getbitu(buff   ,i*8,8));
    for (i=0,j=118;i<11;i++,j+=8) setbitu(crc_buff,j,8,getbitu(buff+16,i*8,8));
    if (rtk_crc24q(crc_buff,25)!=getbitu(buff+16,82,24)) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx gal page crc error: sat=%2d\n",sat);
        #endif
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
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx gal svid error: sat=%2d %2d\n",sat,eph.sat);
        #endif
        return -1;
    }
    if (!strstr(raw->opt,"-EPHALL")) {
        if (eph.iode==raw->nav.eph[sat-1].iode&& /* unchanged */
            timediff(eph.toe,raw->nav.eph[sat-1].toe)==0.0&&
            timediff(eph.toc,raw->nav.eph[sat-1].toc)==0.0) return 0;
    }

    for(int i=0; i<satellites_array->size(); i++)
    {
    	if((*satellites_array)[i].satno==sat)
    	{
    	  (*satellites_array)[i].eph = eph;
    	} else
    	{
        #ifdef LOG_DECODING_MSGS
    	  printf("couldn't find satellite struct in sat_pos vector to store eph in");
        #endif
      }
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

    #ifdef LOG_DECODING_MSGS
    printf("decode_bds_d2:\n");
    #endif

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
        #ifdef LOG_DECODING_MSGS
        printf("decode_bds_d2 error: pgn=%d %d %d %d %d %d %d %d %d\n",
              pgn1,pgn3,pgn4,pgn5,pgn6,pgn7,pgn8,pgn9,pgn10);
        #endif
        return 0;
    }
    if (sow3!=sow1+6||sow4!=sow3+3||sow5!=sow4+3||sow6!=sow5+3||
        sow7!=sow6+3||sow8!=sow7+3||sow9!=sow8+3||sow10!=sow9+3) {
        #ifdef LOG_DECODING_MSGS
        printf("decode_bds_d2 error: sow=%d %d %d %d %d %d %d %d %d\n",
              sow1,sow3,sow4,sow5,sow6,sow7,sow8,sow9,sow10);
        #endif
        return 0;
    }
    if (toc_bds!=eph->toes) {
        #ifdef LOG_DECODING_MSGS
        printf("decode_bds_d2 error: toe=%.0f toc=%.0f\n",eph->toes,toc_bds);
        #endif
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
static int decode_cnav(raw_t *raw, int sat, int off, std::vector<sat_pos> *satellites_array)
{
    eph_t eph={0};
    unsigned int words[10];
    int i,id,pgn,prn;
    unsigned char *p=raw->buff+6+off;

    if (raw->len<48+off) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw->len);
        #endif
        return -1;
    }
    for (i=0;i<10;i++,p+=4) words[i]=U4(p)&0x3FFFFFFF; /* 30 bits */

    satsys(sat,&prn);
    id=(words[0]>>12)&0x07; /* subframe id (3bit) */
    if (id<1||5<id) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx subfrm id error: sat=%2d\n",sat);
        #endif
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
            #ifdef LOG_DECODING_MSGS
            printf("ubx rawsfrbx page number error: sat=%2d\n",sat);
            #endif
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

    for(int i=0; i<satellites_array->size(); i++)
    {
    	if((*satellites_array)[i].satno==sat)
    	{
    	  (*satellites_array)[i].eph = eph;
    	} else
    	{
        #ifdef LOG_DECODING_MSGS
    	  printf("couldn't find satellite struct in sat_pos vector to store eph in");
        #endif
      }
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

    #ifdef LOG_DECODING_MSGS
    printf("decode_glostr:\n");
    #endif

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
        #ifdef LOG_DECODING_MSGS
        printf("decode_glostr error: frn=%d %d %d %d %d\n",frn1,frn2,frn3,frn4);
        #endif
        return 0;
    }
    if (!(geph->sat=satno(SYS_GLO,slot))) {
        #ifdef LOG_DECODING_MSGS
        printf("decode_glostr error: slot=%d\n",slot);
        #endif
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
static int decode_gnav(raw_t *raw, int sat, int off, int frq, std::vector<sat_pos> *satellites_array)
{
    geph_t geph={0};
    int i,j,k,m,prn;
    unsigned char *p=raw->buff+6+off,buff[64],*fid;

    satsys(sat,&prn);

    if (raw->len<24+off) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx gnav length error: len=%d\n",raw->len);
        #endif
        return -1;
    }
    for (i=k=0;i<4;i++,p+=4) for (j=0;j<4;j++) {
        buff[k++]=p[3-j];
    }
    /* test hamming of glonass string */
    if (!test_glostr(buff)) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx glo string hamming error: sat=%2d\n",sat);
        #endif
        return -1;
    }
    m=getbitu(buff,1,4);
    if (m<1||15<m) {
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx glo string no error: sat=%2d\n",sat);
        #endif
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


    for(int i=0; i<satellites_array->size(); i++)
    {
    	if((*satellites_array)[i].satno==sat)
    	{
    	  (*satellites_array)[i].geph = geph;
    	}
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
        #ifdef LOG_DECODING_MSGS
        printf("ubx rawsfrbx snav length error: len=%d\n",raw->len);
        #endif
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


static void init_sat_pos(sat_pos *isp)
{
  eph_t eph0 ={0,-1,-1};

  isp->SNR=0;
  isp->ion_params[0]=0;
  isp->ion_params[1]=0;
  isp->ion_params[2]=0;
  isp->ion_params[3]=0;
  isp->ion_params[4]=0;
  isp->ion_params[5]=0;
  isp->ion_params[6]=0;
  isp->ion_params[7]=0;
  isp->pos[0]=0;
  isp->pos[1]=0;
  isp->pos[2]=0;
  isp->pseudo_range_observed=0;
  isp->pseudo_range_basestation_correction=0;
  isp->pseudo_range_corrected=0;

  isp->eph = eph0;
}

/* decode ubx-rxm-sfrbx: raw subframe data (ref [3][4][5]) -------------------*/
static int decode_rxmsfrbx(raw_t *raw, std::vector<sat_pos> *satellites_array)
{
  //printf("decode rxmsfrbx");
  int prn,sat,sys;
  unsigned char *p=raw->buff+6;

  #ifdef LOG_DECODING_MSGS
  printf("decode_rxmsfrbx: len=%d\n",raw->len);
  #endif

  if (raw->outtype) {
      #ifdef LOG_DECODING_MSGS
      sprintf(raw->msgtype,"UBX RXM-SFRBX (%4d): sys=%d prn=%3d",raw->len,
              U1(p),U1(p+1));
      #endif
  }
  if (!(sys=ubx_sys(U1(p)))) {
      #ifdef LOG_DECODING_MSGS
      printf("ubx rxmsfrbx sys id error: sys=%d\n",U1(p));
      #endif
      return -1;
  }
  prn=U1(p+1)+(sys==SYS_QZS?192:0);
  if (!(sat=satno(sys,prn))) {
      if (sys==SYS_GLO&&prn==255) {
          return 0; /* suppress error for unknown glo satellite */
      }
      #ifdef LOG_DECODING_MSGS
      printf("ubx rxmsfrbx sat number error: sys=%d prn=%d\n",sys,prn);
      #endif
      return -1;
  }

  // sync satellite array with found sats in data frames
  sat_pos sp(sat);
  if(satellites_array->size() == 0)
  {
    init_sat_pos(&sp);
    satellites_array->push_back(sp);
  }

  if(std::find_if(satellites_array->begin(), satellites_array->end(), sp) != satellites_array->end())
  {
    #ifdef LOG_DECODING_MSGS
    printf("satno %d already in sat array \n", sat);
    #endif
  } else
  {
    init_sat_pos(&sp);
    satellites_array->push_back(sp);
  }

  #ifdef LOG_DECODING_MSGS
  printf("%d satellites, current frame satno %d \n" , satellites_array->size(), sat);
  #endif

  switch (sys) {
      case SYS_GPS: return decode_nav (raw,sat,8, satellites_array);
      case SYS_QZS: return decode_nav (raw,sat,8, satellites_array);
      case SYS_GAL: return decode_enav(raw,sat,8, satellites_array);
      case SYS_CMP: return decode_cnav(raw,sat,8, satellites_array);
      case SYS_GLO: return decode_gnav(raw,sat,8,U1(p+3), satellites_array);
      case SYS_SBS: return decode_snav(raw,sat,8);
  }
  return 0;
}

/* decode ubx-rxm-rawx: multi-gnss raw measurement data (ref [3][4][5]) ------*/
static int decode_rxmrawx(raw_t *raw, std::vector<sat_pos> *satellites_array)
{
    //printf("decode rxmrawx");
    gtime_t time;
    unsigned char *p=raw->buff+6;
    char *q,tstr[64];
    double tow,P,L,D,tn,tadj=0.0,toff=0.0;
    int i,j,k,f,sys,prn,sat,code,slip,halfv,halfc,LLI,n=0,std_slip=0;
    int week,nmeas,ver,gnss,svid,sigid,frqid,lockt,cn0,cpstd,tstat;

    #ifdef LOG_DECODING_MSGS
    printf("decode_rxmrawx: len=%d\n",raw->len);
    #endif

    if (raw->len<24) {

        #ifdef LOG_DECODING_MSGS
        printf("ubx rxmrawx length error: len=%d\n",raw->len);
        #endif
        return -1;
    }
    tow  =R8(p   ); /* rcvTow (s) */
    week =U2(p+ 8); /* week */
    nmeas=U1(p+11); /* numMeas */
    ver  =U1(p+13); /* version ([5] 5.15.3.1) */

    if (raw->len<24+32*nmeas) {

        #ifdef LOG_DECODING_MSGS
        printf("ubx rxmrawx length error: len=%d nmeas=%d\n",raw->len,nmeas);
        #endif
        return -1;
    }
    if (week==0) {

        #ifdef LOG_DECODING_MSGS
        printf("ubx rxmrawx week=0 error: len=%d nmeas=%d\n",raw->len,nmeas);
        #endif
        return 0;
    }
    time=gpst2time(week,tow);

    if (raw->outtype) {
        time2str(time,tstr,2);
        sprintf(raw->msgtype,"UBX RXM-RAWX  (%4d): time=%s nmeas=%d ver=%d",
                raw->len,tstr,nmeas,ver);
    }
    /* time tag adjustment option (-TADJ) */
    if ((q=strstr(raw->opt,"-TADJ="))) {
        sscanf(q,"-TADJ=%lf",&tadj);
    }
    /* slip theshold of std-dev of carreir-phase (-STD_SLIP) */
    if ((q=strstr(raw->opt,"-STD_SLIP="))) {
        sscanf(q,"-STD_SLIP=%d",&std_slip);
    }
    /* time tag adjustment */
    if (tadj>0.0) {
        tn=time2gpst(time,&week)/tadj;
        toff=(tn-floor(tn+0.5))*tadj;
        time=timeadd(time,-toff);
    }
    for (i=0,p+=16;i<nmeas&&n<MAXOBS;i++,p+=32) {
        P    =R8(p   );    /* prMes (m) */
        L    =R8(p+ 8);    /* cpMes (cyc) */
        D    =R4(p+16);    /* doMes (hz) */
        gnss =U1(p+20);    /* gnssId */
        svid =U1(p+21);    /* svId */
        sigid=U1(p+22);    /* sigId ([5] 5.15.3.1) */
        frqid=U1(p+23);    /* freqId (fcn + 7) */
        lockt=U2(p+24);    /* locktime (ms) */
        cn0  =U1(p+26);    /* cn0 (dBHz) */
        cpstd=U1(p+28)&15; /* cpStdev (m) */
        tstat=U1(p+30);    /* trkStat */
        if (!(tstat&1)) P=0.0;
        if (!(tstat&2)||L==-0.5||cpstd>CPSTD_VALID) L=0.0;

        if (!(sys=ubx_sys(gnss))) {
            #ifdef LOG_DECODING_MSGS
            printf("ubx rxmrawx: system error gnss=%d\n", gnss);
            #endif
            continue;
        }
        prn=svid+(sys==SYS_QZS?192:0);
        if (!(sat=satno(sys,prn))) {
            if (sys==SYS_GLO&&prn==255) {
                continue; /* suppress warning for unknown glo satellite */
            }
            #ifdef LOG_DECODING_MSGS
            printf("ubx rxmrawx sat number error: sys=%2d prn=%2d\n",sys,prn);
            #endif
            continue;
        }
        if (ver>=1) {
            code=ubx_sig(sys,sigid);
        }
        else {
            code=(sys==SYS_CMP)?CODE_L1I:((sys==SYS_GAL)?CODE_L1X:CODE_L1C);
        }
        /* signal index in obs data */
        f=sig_idx(sys,code);

        if (f==0||f>NFREQ+NEXOBS) {
            #ifdef LOG_DECODING_MSGS
            printf("ubx rxmrawx signal error: sat=%2d sigid=%d\n",sat,sigid);
            #endif
            continue;
        }
        /* offset by time tag adjustment */
        if (toff!=0.0) {
            P-=toff*CLIGHT;
            L-=toff*sig_freq(sys,f,frqid-7);
        }
        halfv=tstat&4?1:0; /* half cycle valid */
        halfc=tstat&8?1:0; /* half cycle subtracted from phase */
        slip=lockt==0||lockt*1E-3<raw->lockt[sat-1][f-1]||
             halfc!=raw->halfc[sat-1][f-1]||(std_slip&&cpstd>=std_slip);
        raw->lockt[sat-1][f-1]=lockt*1E-3;
        raw->halfc[sat-1][f-1]=halfc;
        LLI=(slip?LLI_SLIP:0)|(!halfv?LLI_HALFC:0);

        //sync satellite array with found sats in data frames
        sat_pos sp(sat);
        if(satellites_array->size() == 0)
        {
          init_sat_pos(&sp);
          satellites_array->push_back(sp);
        }

        if(std::find_if(satellites_array->begin(), satellites_array->end(), sp) != satellites_array->end())
        {
          #ifdef LOG_DECODING_MSGS
          printf("satno %d already in sat array \n", sat);
          #endif
        } else
        {
          init_sat_pos(&sp);
          satellites_array->push_back(sp);
        }

        for (j=0;j<n;j++) {
            if (raw->obs.data[j].sat==sat) break;
        }
        if (j>=n) {
            raw->obs.data[n].time=time;
            raw->obs.data[n].sat=sat;
            raw->obs.data[n].rcv=0;
            for (k=0;k<NFREQ+NEXOBS;k++) {
                raw->obs.data[n].L[k]=raw->obs.data[n].P[k]=0.0;
                raw->obs.data[n].D[k]=0.0;
                raw->obs.data[n].SNR[k]=raw->obs.data[n].LLI[k]=0;
                raw->obs.data[n].code[k]=CODE_NONE;
            }
            n++;
        }
        raw->obs.data[j].L[f-1]=L;
        raw->obs.data[j].P[f-1]=P;
        raw->obs.data[j].D[f-1]=(float)D;
        raw->obs.data[j].SNR[f-1]=(unsigned char)(cn0*4);
        raw->obs.data[j].LLI[f-1]=(unsigned char)LLI;
        raw->obs.data[j].code[f-1]=(unsigned char)code;
    }

    for(int i=0; i<satellites_array->size(); i++)
    {
    	if((*satellites_array)[i].satno==sat)
    	{
    	  (*satellites_array)[i].pseudo_range_observed = P;
    	}
    }
    raw->time=time;
    raw->obs.n=n;
    return 1;
}
