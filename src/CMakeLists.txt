cmake_minimum_required(VERSION 3.2)

add_library (gpsCore gpsCore.cpp)

target_include_directories (gpsCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
