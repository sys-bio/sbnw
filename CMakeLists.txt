cmake_minimum_required(VERSION 3.15)
project(SBNW)

set(SBNW_MAJOR_VER 1)
set(SBNW_MINOR_VER 3)
set(SBNW_PATCHLEVEL 27)

set(SBNW_VER_STRING "${SBNW_MAJOR_VER}.${SBNW_MINOR_VER}.${SBNW_PATCHLEVEL}")

set(SBNW_LINK_TO_STATIC_LIBSBML ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(FPIC_FLAG "-fPIC")
else ()
    set(FPIC_FLAG)
endif ()

set(SAGITTARIUS_BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})

if (MINGW)
    set(SBNW_MINGW ON)
    set(SBNW_NTDDI_VERSION "NTDDI_WIN7" CACHE STRING "Latest Windows version to include support for")
    add_definitions(-DNTDDI_VERSION=${SBNW_NTDDI_VERSION})
else (MINGW)
    set(SBNW_MINGW OFF)
endif (MINGW)

# Python
set(ENABLE_PYTHON3_BINDINGS OFF CACHE BOOL "Enable Python 3 bindings")
set(ENABLE_PYTHON2_BINDINGS OFF CACHE BOOL "Enable Python 2 bindings")

if (ENABLE_PYTHON3_BINDINGS OR ENABLE_PYTHON2_BINDINGS)
    set(ENABLE_PYTHON_BINDINGS ON)
else ()
    set(ENABLE_PYTHON_BINDINGS OFF)
endif ()

#MagickWand
set(LINK_WITH_MAGICK OFF CACHE BOOL "Link with Image Magick?")
if (LINK_WITH_MAGICK)
    set(MAGICK_INCLUDE "/usr/include/ImageMagick")
    set(MAGICK_LIBDIR "/usr/lib")
    set(MAGICK_LIBS MagickWand)
endif ()
set(SBNW_DEBUG_LEVEL 2 CACHE STRING "Verbosity of debug checks, must be greater than zero")
set(SBNW_LINK_TO_STATIC_LIBSBML OFF CACHE BOOL "Link to a static version of libSBML where available")

#User Configuration
if (${WIN32})
    #Internal Options
    set(SBNW_WIN_COPY_DEPS_TO_INSTALL ON CACHE BOOL "When true, libraries that graphfab depends on (libSBML and its dependencies) will be copied to the graphfab installation directory")
    #libSBML
    set(LIBSBML_PREFIX "C:/Program Files (x86)/libSBML" CACHE FILEPATH "libSBML root directory (contains lib, include, etc.)")
else ()
    #Important paths
    set(SAGITTARIUS_BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})

    #Internal Options
    set(SBNW_WIN_COPY_DEPS_TO_INSTALL OFF CACHE BOOL "Should be off for Linux")
    set(ENABLE_FULL_RPATH 1 CACHE BOOL "Enable full RPath in generated executable code")

    #libSBML
    set(LIBSBML_PREFIX "/usr/local" CACHE FILEPATH "libSBML root directory (contains lib, include, etc.)")

    #C/C++ compiler flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline") # -pedantic -Wextra
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif ()

if (NOT EXISTS "${LIBSBML_PREFIX}")
    message(FATAL_ERROR "Directory given for LIBSBML_PREFIX variable (\"${LIBSBML_PREFIX}\") does not exist")
endif ()

# set libsbml directories and dependency variables based on LIBSBML_PREFIX
set(LIBSBML_INCLUDE ${LIBSBML_PREFIX}/include)
set(LIBSBML_LIBDIR ${LIBSBML_PREFIX}/lib)

set(LIBSBML_EXTRA_LIBS_DIR
        "${LIBSBML_LIBDIR}/lib"
        CACHE FILEPATH
        "Path to folder containing libbz2.lib, libiconv.lib, libxml2.lib and zdll.lib"
        )



set(BZ_LIB "${LIBSBML_EXTRA_LIBS_DIR}/libbz2.lib")
set(ICONV_LIB "${LIBSBML_EXTRA_LIBS_DIR}/libiconv.lib")
set(LIBSBML2_LIB "${LIBSBML_EXTRA_LIBS_DIR}/libxml2.lib")
set(ZDLL "${LIBSBML_EXTRA_LIBS_DIR}/zdll.lib")

# collect deps into varibale
list(APPEND LIBSBML_EXTRA_LIBS
        ${BZ_LIB}
        ${ICONV_LIB}
        ${LIBSBML2_LIB}
        ${ZDLL}
        )

set(SBNW_SPYDER_DIST OFF CACHE BOOL "Install Spyder plugin files")

set(BUILD_STATIC_LIB ON CACHE BOOL "Build the static version of the library")

# GTest
find_package(GTest)
if (GTEST_FOUND)
    set(WITH_GTEST ON CACHE BOOL "Use gtest framework")
else ()
    set(WITH_GTEST OFF CACHE BOOL "Use gtest framework")
endif ()

set(ENABLE_PYTHON FALSE CACHE BOOL "Enable Python bindings")
if (ENABLE_PYTHON)
    find_package(PythonInterp)
    if (PYTHONINTERP_FOUND)
        set(Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION_STRING})
    else ()
        message(ERROR FATAL "Unable to find Python")
    endif ()
endif ()

# Find Python libs
find_package(PythonLibs)
message(STATUS "Using Python include directory ${PYTHON_INCLUDE_DIR}")

#Dependencies

#SBML
find_library(LIBSBML_LIBRARY sbml libsbml HINTS ${LIBSBML_PREFIX}/lib)
find_library(LIBSBML_STATIC_LIBRARY sbml-static REQUIRED libsbml-static HINTS ${LIBSBML_PREFIX}/lib)
find_path(LIBSBML_INCLUDE_DIR sbml/SBMLTypes.h PATHS ${LIBSBML_PREFIX}/include)
include_directories(${LIBSBML_INCLUDE_DIR})

#Image Magick
if (LINK_WITH_MAGICK)
    set(SBNW_USE_MAGICK 1)
    include_directories(${MAGICK_INCLUDE})
else ()
    set(SBNW_USE_MAGICK 0)
endif ()

if (ENABLE_FULL_RPATH) #http://www.cmake.org/Wiki/CMake_RPATH_handling
    SET(CMAKE_SKIP_BUILD_RPATH FALSE)

    SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    IF ("${isSystemDir}" STREQUAL "-1")
        SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    ENDIF ("${isSystemDir}" STREQUAL "-1")
endif ()

#add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}) #we have to do this to get the generated header

add_subdirectory(graphfab)

#add_subdirectory(sandbox)
#
#add_subdirectory(icons)

add_subdirectory(testcases)

if (WITH_GTEST)
    add_subdirectory(test)
endif ()


message(STATUS "
CMAKE_CURRENT_SOURCE_DIR                ${CMAKE_CURRENT_SOURCE_DIR}
CMAKE_CURRENT_BINARY_DIR                ${CMAKE_CURRENT_BINARY_DIR}
LIBSBML_PREFIX                          ${LIBSBML_PREFIX}
LIBSBML_LIBRARY                         ${LIBSBML_LIBRARY}
LIBSBML_STATIC_LIBRARY                  ${LIBSBML_STATIC_LIBRARY}
LIBSBML_INCLUDE_DIR                     ${LIBSBML_INCLUDE_DIR}
LIBSBML_EXTRA_LIBS                      ${LIBSBML_EXTRA_LIBS}
SBNW_USE_MAGICK                         ${SBNW_USE_MAGICK}
LIBSBML_EXTRA_LIBS_DIR                  ${LIBSBML_EXTRA_LIBS_DIR}
CMAKE_SKIP_BUILD_RPATH                  ${CMAKE_SKIP_BUILD_RPATH}
CMAKE_BUILD_WITH_INSTALL_RPATH          ${CMAKE_BUILD_WITH_INSTALL_RPATH}
CMAKE_INSTALL_RPATH                     ${CMAKE_INSTALL_RPATH}
CMAKE_INSTALL_RPATH_USE_LINK_PATH       ${CMAKE_INSTALL_RPATH_USE_LINK_PATH}
")

