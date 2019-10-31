
/* decode gps and qzss navigation data ---------------------------------------*/
static int decode_nav(raw_sfrbx_msg *raw, int sat, int off)
{
    unsigned int words[10];
    int i,id;
    unsigned char *p=raw.data_msg_buff+6+off;

    if (raw.len<48+off) {
        trace(2,"ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw.len);
        return -1;
    }
    if ((U4(p)>>24)==PREAMB_CNAV) {
        trace(3,"ubx rawsfrbx cnav not supported sat=%d prn=%d\n",sat,
              (U4(p)>>18)&0x3F);
        return 0;
    }
    for (i=0;i<10;i++,p+=4) words[i]=U4(p)>>6; /* 24 bits without parity */

    id=(words[1]>>2)&7;
    if (id<1||5<id) {
        trace(2,"ubx rawsfrbx subfrm id error: sat=%2d id=%d len=%d\n",sat,id,
              raw.len);
        return -1;
    }
    for (i=0;i<10;i++) {
        setbitu(raw.subfrm[sat-1]+(id-1)*30,i*24,24,words[i]);
    }
    if (id==3) return decode_ephem(sat,raw);
    if (id==4) return decode_alm1 (sat,raw);
    if (id==5) return decode_alm2 (sat,raw);
    return 0;
}
/* decode galileo navigation data --------------------------------------------*/
static int decode_enav(raw_sfrbx_msg *raw, int sat, int off)
{
    eph_t eph={0};
    unsigned char *p=raw.data_msg_buff+6+off,data_msg_buff[32],crc_data_msg_buff[26]={0};
    int i,j,k,part1,page1,part2,page2,type;

    if (raw.len<44+off) {
        trace(2,"ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw.len);
        return -1;
    }
    for (i=k=0;i<8;i++,p+=4) for (j=0;j<4;j++) {
        data_msg_buff[k++]=p[3-j];
    }
    part1=getbitu(data_msg_buff   ,0,1);
    page1=getbitu(data_msg_buff   ,1,1);
    part2=getbitu(data_msg_buff+16,0,1);
    page2=getbitu(data_msg_buff+16,1,1);

    /* skip alert page */
    if (page1==1||page2==1) return 0;

    /* test even-odd parts */
    if (part1!=0||part2!=1) {
        trace(2,"ubx rawsfrbx gal page even/odd error: sat=%2d\n",sat);
        return -1;
    }
    /* test crc (4(pad) + 114 + 82 bits) */
    for (i=0,j=  4;i<15;i++,j+=8) setbitu(crc_data_msg_buff,j,8,getbitu(data_msg_buff   ,i*8,8));
    for (i=0,j=118;i<11;i++,j+=8) setbitu(crc_data_msg_buff,j,8,getbitu(data_msg_buff+16,i*8,8));
    if (rtk_crc24q(crc_data_msg_buff,25)!=getbitu(data_msg_buff+16,82,24)) {
        trace(2,"ubx rawsfrbx gal page crc error: sat=%2d\n",sat);
        return -1;
    }
    type=getbitu(data_msg_buff,2,6); /* word type */

    /* skip word except for ephemeris, iono, utc parameters */
    if (type>6) return 0;

    /* clear word 0-6 flags */
    if (type==2) raw.subfrm[sat-1][112]=0;

    /* save page data (112 + 16 bits) to frame data_msg_buffer */
    k=type*16;
    for (i=0,j=2;i<14;i++,j+=8) raw.subfrm[sat-1][k++]=getbitu(data_msg_buff   ,j,8);
    for (i=0,j=2;i< 2;i++,j+=8) raw.subfrm[sat-1][k++]=getbitu(data_msg_buff+16,j,8);

    /* test word 0-6 flags */
    raw.subfrm[sat-1][112]|=(1<<type);
    if (raw.subfrm[sat-1][112]!=0x7F) return 0;

    if (strstr(raw.opt,"-GALFNAV")) {
        return 0;
    }
    /* decode galileo inav ephemeris */
    if (!decode_gal_inav(raw.subfrm[sat-1],&eph)) {
        return 0;
    }
    /* test svid consistency */
    if (eph.sat!=sat) {
        trace(2,"ubx rawsfrbx gal svid error: sat=%2d %2d\n",sat,eph.sat);
        return -1;
    }
    if (!strstr(raw.opt,"-EPHALL")) {
        if (eph.iode==raw.nav.eph[sat-1].iode&& /* unchanged */
            timediff(eph.toe,raw.nav.eph[sat-1].toe)==0.0&&
            timediff(eph.toc,raw.nav.eph[sat-1].toc)==0.0) return 0;
    }
    eph.sat=sat;
    raw.nav.eph[sat-1]=eph;
    raw.ephsat=sat;
    return 2;
}
/* decode beidou navigation data ---------------------------------------------*/
static int decode_cnav(raw_sfrbx_msg *raw, int sat, int off)
{
    eph_t eph={0};
    unsigned int words[10];
    int i,id,pgn,prn;
    unsigned char *p=raw.data_msg_buff+6+off;

    if (raw.len<48+off) {
        trace(2,"ubx rawsfrbx length error: sat=%d len=%d\n",sat,raw.len);
        return -1;
    }
    for (i=0;i<10;i++,p+=4) words[i]=U4(p)&0x3FFFFFFF; /* 30 bits */

    satsys(sat,&prn);
    id=(words[0]>>12)&0x07; /* subframe id (3bit) */
    if (id<1||5<id) {
        trace(2,"ubx rawsfrbx subfrm id error: sat=%2d\n",sat);
        return -1;
    }
    if (prn>5&&prn<59) { /* IGSO/MEO */

        for (i=0;i<10;i++) {
            setbitu(raw.subfrm[sat-1]+(id-1)*38,i*30,30,words[i]);
        }
        if (id!=3) return 0;

        /* decode beidou D1 ephemeris */
        if (!decode_bds_d1(raw.subfrm[sat-1],&eph)) return 0;
    }
    else { /* GEO (C01-05, C59-63) */
        if (id!=1) return 0;

        /* subframe 1 */
        pgn=(words[1]>>14)&0x0F; /* page number (4bit) */
        if (pgn<1||10<pgn) {
            trace(2,"ubx rawsfrbx page number error: sat=%2d\n",sat);
            return -1;
        }
        for (i=0;i<10;i++) {
            setbitu(raw.subfrm[sat-1]+(pgn-1)*38,i*30,30,words[i]);
        }
        if (pgn!=10) return 0;

        /* decode beidou D2 ephemeris */
        if (!decode_bds_d2(raw.subfrm[sat-1],&eph)) return 0;
    }
    if (!strstr(raw.opt,"-EPHALL")) {
        if (timediff(eph.toe,raw.nav.eph[sat-1].toe)==0.0&&
            eph.iode==raw.nav.eph[sat-1].iode&&
            eph.iodc==raw.nav.eph[sat-1].iodc) return 0; /* unchanged */
    }
    eph.sat=sat;
    raw.nav.eph[sat-1]=eph;
    raw.ephsat=sat;
    return 2;
}
/* decode glonass navigation data --------------------------------------------*/
static int decode_gnav(raw_sfrbx_msg *raw, int sat, int off, int frq)
{
    geph_t geph={0};
    int i,j,k,m,prn;
    unsigned char *p=raw.data_msg_buff+6+off,data_msg_buff[64],*fid;

    satsys(sat,&prn);

    if (raw.len<24+off) {
        trace(2,"ubx rawsfrbx gnav length error: len=%d\n",raw.len);
        return -1;
    }
    for (i=k=0;i<4;i++,p+=4) for (j=0;j<4;j++) {
        data_msg_buff[k++]=p[3-j];
    }
    /* test hamming of glonass string */
    if (!test_glostr(data_msg_buff)) {
        trace(2,"ubx rawsfrbx glo string hamming error: sat=%2d\n",sat);
        return -1;
    }
    m=getbitu(data_msg_buff,1,4);
    if (m<1||15<m) {
        trace(2,"ubx rawsfrbx glo string no error: sat=%2d\n",sat);
        return -1;
    }
    /* flush frame data_msg_buffer if frame-id changed */
    fid=raw.subfrm[sat-1]+150;
    if (fid[0]!=data_msg_buff[12]||fid[1]!=data_msg_buff[13]) {
        for (i=0;i<4;i++) memset(raw.subfrm[sat-1]+i*10,0,10);
        memcpy(fid,data_msg_buff+12,2); /* save frame-id */
    }
    memcpy(raw.subfrm[sat-1]+(m-1)*10,data_msg_buff,10);

    if (m!=4) return 0;

    /* decode glonass ephemeris strings */
    geph.tof=raw.time;
    if (!decode_glostr(raw.subfrm[sat-1],&geph)||geph.sat!=sat) return 0;
    geph.frq=frq-7;

    if (!strstr(raw.opt,"-EPHALL")) {
        if (geph.iode==raw.nav.geph[prn-1].iode) return 0; /* unchanged */
    }
    raw.nav.geph[prn-1]=geph;
    raw.ephsat=sat;
    return 2;
}
/* decode sbas navigation data -----------------------------------------------*/
static int decode_snav(raw_sfrbx_msg *raw, int sat, int off)
{
    int i,j,k,prn,tow,week;
    unsigned char *p=raw.data_msg_buff+6+off,data_msg_buff[64];

    if (raw.len<40+off) {
        trace(2,"ubx rawsfrbx snav length error: len=%d\n",raw.len);
        return -1;
    }
    tow=(int)time2gpst(timeadd(raw.time,-1.0),&week);
    satsys(sat,&prn);
    raw.sbsmsg.prn=prn;
    raw.sbsmsg.tow=tow;
    raw.sbsmsg.week=week;
    for (i=k=0;i<8;i++,p+=4) for (j=0;j<4;j++) {
        data_msg_buff[k++]=p[3-j];
    }
    memcpy(raw.sbsmsg.msg,data_msg_buff,29);
    raw.sbsmsg.msg[28]&=0xC0;
    return 3;
}
/* decode ubx-rxm-sfrbx: raw subframe data (ref [3][4][5]) -------------------*/
static int decode_rxmsfrbx(raw_sfrbx_msg *raw)
{
    int prn,sat,sys;
    unsigned char *p=raw.data_msg_buff+6;

    trace(4,"decode_rxmsfrbx: len=%d\n",raw.len);

    if (raw.outtype) {
        sprintf(raw.msgtype,"UBX RXM-SFRBX (%4d): sys=%d prn=%3d",raw.len,
                U1(p),U1(p+1));
    }
    if (!(sys=ubx_sys(U1(p)))) {
        trace(2,"ubx rxmsfrbx sys id error: sys=%d\n",U1(p));
        return -1;
    }
    prn=U1(p+1)+(sys==SYS_QZS?192:0);
    if (!(sat=satno(sys,prn))) {
        if (sys==SYS_GLO&&prn==255) {
            return 0; /* suppress error for unknown glo satellite */
        }
        trace(2,"ubx rxmsfrbx sat number error: sys=%d prn=%d\n",sys,prn);
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
