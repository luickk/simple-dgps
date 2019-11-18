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

#include <wiringSerial.h>
#include <wiringPi.h>

#include "src/ubx_rcv.cpp"

#include "src/gps_differential.cpp"

M8T receiver;

void print_sata_stats(std::vector<sat_pos> *satellites_array)
{
  while(true)
  {
      usleep(10000000);
      receiver.print_sat_pos_array(satellites_array);
  }
}

int main(int argc, char **argv)
{
  int fd;
  char input;
  dgps differential_gps;

  double bs_lat_lon_pos[3] = {53.2734, -7.77832031, 52};
  // exact position of base station in ecef coordinates
  double ecef_base_station_position[3] = {0, 0, 0}; // x, y, z
  latloheightn2ecefpos(bs_lat_lon_pos, ecef_base_station_position);

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

  // not initiating raw struct since no storage is foreseen
  // init_raw(&raw_data);

  vector<sat_pos> satellites_array;

  vector<sat_pos> rsp;

  std::thread print_sata_stats_thread (print_sata_stats, &satellites_array);

  while(1)
  {
    while (serialDataAvail (fd))
    {
      input = serialGetchar (fd);

      // std::cout << input << std::endl;
      // std::cout << "-----" << std::endl;


      receiver.decode_ubx_msgs(&raw_data, &satellites_array, input);

      //std::cout << raw_data.obs.data->P[1] << std::endl;

      // std::cout << satellites_array.size() << std::endl;

      // applying pseudo range correction
      // differential_gps.apply_prc(&satellites_array, &rsp, ecef_base_station_position);

      if(input==-1){
        goto REINIT;
      }
    }
  }



  return 0;
}
