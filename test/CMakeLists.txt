cmake_minimum_required(VERSION 3.2)
project(simple-dgps)


add_executable(coreTest coreTest.cpp)

target_link_libraries (coreTest LINK_PUBLIC SimpleDGps)

install(TARGETS coreTest DESTINATION ${DIVISIBLE_INSTALL_BIN_DIR})
