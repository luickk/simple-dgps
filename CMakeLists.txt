cmake_minimum_required(VERSION 3.2)

project(simple-dgps)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(DIVISIBLE_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(DIVISIBLE_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(DIVISIBLE_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(${DIVISIBLE_INSTALL_INCLUDE_DIR}/cc.ublox.commsdsl)
include_directories(${DIVISIBLE_INSTALL_INCLUDE_DIR})
link_directories(${DIVISIBLE_INSTALL_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)