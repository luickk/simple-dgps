cmake_minimum_required(VERSION 3.2)

project(simple-dgps)

set(UBLOX_COMMDSL_PROJECT_PATH "/Users/luickklippel/Documents/projekte/cc.ublox.commsdsl/")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(DIVISIBLE_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(DIVISIBLE_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
include_directories("${CMAKE_SOURCE_DIR}/src")
add_subdirectory(test)
