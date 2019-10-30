
struct raw_sfrbx_msg
{
  double time_stamp;
  char data_msg_buf[64]
};

struct ephemeris_data
{
  int sat_id;

  double week;
  double t_gd;
  double IODC;
  double t_oc;
  double a_f;

  double IODE2;
  double C_rs;
  double dn;
  double M_0;
  double C_uc;
  double e;
  double C_us;
  double sqrtA;
  double t_oe;
  double C_ic;
  double OMEGA_0;
  double C_is;
  double i_0;
  double C_rc;
  double omega;
  double MEGA_dot;
  double IODE3;
  double IDOT;
};
