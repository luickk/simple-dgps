#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "commsdsl::commsdsl" for configuration ""
set_property(TARGET commsdsl::commsdsl APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(commsdsl::commsdsl PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libcommsdsl.3.5.2.dylib"
  IMPORTED_SONAME_NOCONFIG "@rpath/libcommsdsl.3.5.2.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS commsdsl::commsdsl )
list(APPEND _IMPORT_CHECK_FILES_FOR_commsdsl::commsdsl "${_IMPORT_PREFIX}/lib/libcommsdsl.3.5.2.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
