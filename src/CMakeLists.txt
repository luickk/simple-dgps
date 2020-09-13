cmake_minimum_required(VERSION 3.2)
project(simple-dgps)

FIND_PACKAGE( Boost 1.40 REQUIRED )
message(STATUS "Boost version: ${Boost_VERSION}")
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_library (GpsCore gpsCore.cpp)
add_library (CommCore commCore.cpp)


install(TARGETS GpsCore DESTINATION ${DIVISIBLE_INSTALL_LIB_DIR})
install(TARGETS CommCore DESTINATION ${DIVISIBLE_INSTALL_LIB_DIR})
