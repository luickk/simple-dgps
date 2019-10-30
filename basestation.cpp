#include <sstream>
#include <iostream>
#include <limits>

#include <wiringSerial.h>
#include <wiringPi.h>

int main(int argc, char **argv)
{
  int fd;
  char input;

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

  while(1)
  {
    while (serialDataAvail (fd))
    {
      input = serialGetchar (fd);

      std::cout << input << std::endl;

      std::cout << "-----" << std::endl;

      if(input==-1){
        goto REINIT;
      }
    }
  }

  return 0;
}
