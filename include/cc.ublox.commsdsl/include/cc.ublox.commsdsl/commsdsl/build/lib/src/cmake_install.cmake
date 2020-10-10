# Install script for directory: /Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/externals/commsdsl/lib/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/commsdsl/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/commsdsl/build/lib/src/libcommsdsl.3.5.2.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcommsdsl.3.5.2.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcommsdsl.3.5.2.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcommsdsl.3.5.2.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/commsdsl/build/lib/src/libcommsdsl.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcommsdsl.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcommsdsl.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcommsdsl.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/LibCommsdsl/cmake" TYPE FILE FILES "/Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/commsdsl/build/CMakeFiles/LibCommsdslConfig.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/LibCommsdsl/cmake/LibCommsdslTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/LibCommsdsl/cmake/LibCommsdslTargets.cmake"
         "/Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/commsdsl/build/lib/src/CMakeFiles/Export/lib/LibCommsdsl/cmake/LibCommsdslTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/LibCommsdsl/cmake/LibCommsdslTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/LibCommsdsl/cmake/LibCommsdslTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/LibCommsdsl/cmake" TYPE FILE FILES "/Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/commsdsl/build/lib/src/CMakeFiles/Export/lib/LibCommsdsl/cmake/LibCommsdslTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/LibCommsdsl/cmake" TYPE FILE FILES "/Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/commsdsl/build/lib/src/CMakeFiles/Export/lib/LibCommsdsl/cmake/LibCommsdslTargets-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/LibCommsdsl/cmake" TYPE FILE FILES "/Users/luickklippel/Documents/projekte/simple-dgps/include/cc.ublox.commsdsl/include/cc.ublox.commsdsl/commsdsl/build/LibCommsdslConfigVersion.cmake")
endif()

