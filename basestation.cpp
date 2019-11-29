#include <sstream>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <thread>
#include <dirent.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <wiringSerial.h>
#include <wiringPi.h>

#include "src/sdgps_defines.h"
#include "src/data.h"

#include "src/gps_timing.cpp"

#include "src/gps_sat.cpp"

#include "src/ubx_parsing.cpp"

#include "src/gps_corrections.cpp"

#include "src/nav_data_decoding.cpp"

#include "src/sdgps_conv.cpp"

#include "src/gps_trilateration.cpp"

#include "src/ubx_rcv.cpp"

#include "src/gps_differential.cpp"

M8T receiver;

void print_sata_stats(std::vector<sat_pos> *satellites_array)
{
  while(true)
  {
      usleep(10000000);
      // receiver.print_sat_pos_array(satellites_array);
  }
}

int main(int argc, char **argv)
{
  int fd;
  char input;
  dgps differential_gps;

  double bs_lat_lon_pos[3] = {49.478844, 10.960637, 130};
  // exact position of base station in ecef coordinates
  double ecef_base_station_position[3] = {0, 0, 0}; // x, y, z

  double *ecef_x,*ecef_y,*ecef_z;
  latLonToEcef(bs_lat_lon_pos, ecef_base_station_position);

  std::cout << ecef_base_station_position[0] << ", " << ecef_base_station_position[1] << ", " << ecef_base_station_position[2] << std::endl;

  std::cout << "Starting DGPS basestation" << std::endl;

	if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi") ;
  }

  REINIT:if ((fd = serialOpen ("/dev/ttyS0", 9600)) < 0)
  {
   fprintf (stderr, "Unable to open serial device");
  }

  std::cout << "Opened GPS receiver serial con" << std::endl;

  raw_t raw_data;

  init_raw(&raw_data);

  vector<sat_pos> satellites_array;

  vector<sat_pos> rsp;

  std::thread print_sata_stats_thread (print_sata_stats, &satellites_array);

  while(1)
  {
    while (serialDataAvail (fd))
    {
      input = serialGetchar (fd);

      // copying over data from basestatio sat array to rover for testing purposes
      rsp = satellites_array;

      receiver.decode_ubx_msgs(&raw_data, &satellites_array, input);

      // applying pseudo range correction
      differential_gps.apply_prc(&satellites_array, &rsp, ecef_base_station_position);

      if(input==-1){
        goto REINIT;
      }
    }
  }



  return 0;
}
