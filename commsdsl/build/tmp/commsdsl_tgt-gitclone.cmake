
if(NOT "/Users/luickklippel/Documents/projekte/simple-dgps//commsdsl/build/commsdsl_tgt-gitinfo.txt" IS_NEWER_THAN "/Users/luickklippel/Documents/projekte/simple-dgps//commsdsl/build/commsdsl_tgt-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/Users/luickklippel/Documents/projekte/simple-dgps//commsdsl/build/commsdsl_tgt-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/luickklippel/Documents/projekte/simple-dgps//externals/commsdsl"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/luickklippel/Documents/projekte/simple-dgps//externals/commsdsl'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/local/bin/git"  clone --no-checkout "https://github.com/commschamp/commsdsl.git" "commsdsl"
    WORKING_DIRECTORY "/Users/luickklippel/Documents/projekte/simple-dgps/externals"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/commschamp/commsdsl.git'")
endif()

execute_process(
  COMMAND "/usr/local/bin/git"  checkout v3.5.2 --
  WORKING_DIRECTORY "/Users/luickklippel/Documents/projekte/simple-dgps/externals/commsdsl"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v3.5.2'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/local/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/luickklippel/Documents/projekte/simple-dgps/externals/commsdsl"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/luickklippel/Documents/projekte/simple-dgps/externals/commsdsl'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/Users/luickklippel/Documents/projekte/simple-dgps//commsdsl/build/commsdsl_tgt-gitinfo.txt"
    "/Users/luickklippel/Documents/projekte/simple-dgps//commsdsl/build/commsdsl_tgt-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/luickklippel/Documents/projekte/simple-dgps//commsdsl/build/commsdsl_tgt-gitclone-lastrun.txt'")
endif()

