#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include <wiringSerial.h>

int main ()
{
  int fd ;

  if((fd=serialOpen("/dev/ttyACM0",9600))<0){
    fprintf(stderr,"Unable to open serial device: %s\n",strerror(errno));
    return 1;
  }

  while (1)
  {
    std::cout << serialGetchar(fd) << std::endl;
  }
}
