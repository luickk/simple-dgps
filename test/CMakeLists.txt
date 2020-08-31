cmake_minimum_required(VERSION 3.2)
project(coreTest)

add_executable(coreTest coreTest.cpp)

target_link_libraries (coreTest LINK_PUBLIC GpsCore)

install(TARGETS coreTest DESTINATION ${DIVISIBLE_INSTALL_BIN_DIR})
