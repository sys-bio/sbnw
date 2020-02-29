#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libsbml" for configuration "Release"
set_property(TARGET libsbml APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libsbml PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libsbml.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "D:/sbnw/third_party_libs/libsbml-dependencies/lib/libbz2.lib;D:/sbnw/third_party_libs/libsbml-dependencies/lib/zdll.lib;D:/sbnw/third_party_libs/libsbml/lib/libxml2.lib;WS2_32.lib;D:/sbnw/third_party_libs/libsbml/lib/libiconv.lib;D:/sbnw/third_party_libs/libsbml-dependencies/lib/zdll.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libsbml.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libsbml )
list(APPEND _IMPORT_CHECK_FILES_FOR_libsbml "${_IMPORT_PREFIX}/lib/libsbml.lib" "${_IMPORT_PREFIX}/bin/libsbml.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
