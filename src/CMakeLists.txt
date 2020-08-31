cmake_minimum_required(VERSION 3.2)
project(simple-dgps)

add_library (GpsCore gpsCore.cpp)

install(TARGETS GpsCore DESTINATION ${DIVISIBLE_INSTALL_LIB_DIR})
