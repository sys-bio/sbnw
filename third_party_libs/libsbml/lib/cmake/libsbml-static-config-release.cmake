#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libsbml-static" for configuration "Release"
set_property(TARGET libsbml-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libsbml-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "D:/sbnw/third_party_libs/libsbml-dependencies/lib/libbz2.lib;D:/sbnw/third_party_libs/libsbml-dependencies/lib/zdll.lib;D:/sbnw/third_party_libs/libsbml-dependencies/lib/libxml2.lib;WS2_32.lib;D:/sbnw/third_party_libs/libsbml-dependencies/lib/libiconv.lib;D:/sbnw/third_party_libs/libsbml-dependencies/lib/zdll.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsbml-static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS libsbml-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_libsbml-static "${_IMPORT_PREFIX}/lib/libsbml-static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
