cmake_minimum_required(VERSION 3.2)
project(simple-dgps)

find_package( Boost 1.40 REQUIRED )
message(STATUS "Boost version: ${Boost_VERSION}")
include_directories( ${Boost_INCLUDE_DIR} )

include_directories("${CMAKE_SOURCE_DIR}/src/include/")
include_directories("${CMAKE_SOURCE_DIR}/build/include/cc.ublox.commsdsl/comms_champion/install/include/")
include_directories("${CMAKE_SOURCE_DIR}/include/cc.ublox.commsdsl/output/include/")

add_library (SimpleDGps simpleDGps.cpp)
add_library (CommCore commCore.cpp)


install(TARGETS SimpleDGps DESTINATION ${DIVISIBLE_INSTALL_LIB_DIR})
install(TARGETS CommCore DESTINATION ${DIVISIBLE_INSTALL_LIB_DIR})
