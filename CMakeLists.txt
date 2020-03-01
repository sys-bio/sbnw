# cmake preliminaries
cmake_minimum_required(VERSION 3.15)

# set default install prefix
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/INSTALL CACHE PATH "Directory for sbnw installation")

project(SBNW)

# for the ExternalProject_Add command
include(ExternalProject)

# set version information
set(SBNW_MAJOR_VER 1)
set(SBNW_MINOR_VER 3)
set(SBNW_PATCHLEVEL 27)
set(SBNW_VER_STRING "${SBNW_MAJOR_VER}.${SBNW_MINOR_VER}.${SBNW_PATCHLEVEL}")

# set diretory for third party libs
SET(SBNW_THIRD_PARTY_LIBS ${CMAKE_SOURCE_DIR}/third_party_libs)
SET(LIBSBML_DEPENDENCIES_PREFIX ${SBNW_THIRD_PARTY_LIBS}/libsbml-dependencies)
SET(LIBSBML_PREFIX ${SBNW_THIRD_PARTY_LIBS}/libsbml)
SET(GOOGLETEST_PREFIX ${SBNW_THIRD_PARTY_LIBS}/googletest)

# download and build libsbml-dependencies package with options appropriate for sbnw
ExternalProject_Add(LIBSBML_DEPENDENCIES
        GIT_REPOSITORY
        https://github.com/sbmlteam/libSBML-dependencies
        GIT_TAG
        master
        SOURCE_DIR
        "${CMAKE_BINARY_DIR}/libsbml-dependencies-src"
        BINARY_DIR
        "${CMAKE_BINARY_DIR}/libsbml-dependencies-build"
        CMAKE_ARGS
        -DBUILD_SHARED_LIBS=OFF
        -DWITH_STATIC_RUNTIME=ON
        -DCMAKE_INSTALL_PREFIX=${LIBSBML_DEPENDENCIES_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        )

# download and build libsbml with appropirate options for libsbml
ExternalProject_Add(LIBSBML
        GIT_REPOSITORY
        https://github.com/opencor/libsbml.git
        GIT_TAG
        master
        SOURCE_DIR
        "${CMAKE_BINARY_DIR}/libsbml-src"
        BINARY_DIR
        "${CMAKE_BINARY_DIR}/libsbml-build"
        CMAKE_ARGS
        -DBUILD_SHARED_LIBS=OFF
        -DENABLE_LAYOUT=ON
        -DWITH_STATIC_RUNTIME=OFF
        -DWITH_CPP_NAMESPACES=OFF
        -DCMAKE_INSTALL_PREFIX=${LIBSBML_PREFIX}
        -DLIBSBML_DEPENDENCY_DIR=${LIBSBML_DEPENDENCIES_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        DEPENDS
        LIBSBML_DEPENDENCIES
        )

# download and build googletest
ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
        SOURCE_DIR "${CMAKE_BINARY_DIR}/googletest-src"
        BINARY_DIR "${CMAKE_BINARY_DIR}/googletest-build"
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${GOOGLETEST_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        )

set(BUILD_TESTS "OFF" CACHE BOOL "Build sbnw tests")

# set libsbml directories and dependency variables based on LIBSBML_PREFIX
set(LIBSBML_INCLUDE ${LIBSBML_PREFIX}/include)
set(LIBSBML_LIBDIR ${LIBSBML_PREFIX}/lib)
set(BZ_LIB "${LIBSBML_LIBDIR}/libbz2.lib")
set(ICONV_LIB "${LIBSBML_LIBDIR}/libiconv.lib")
set(LIBSBML2_LIB "${LIBSBML_LIBDIR}/libxml2.lib")
set(ZDLL "${LIBSBML_LIBDIR}/zdll.lib")

# collect deps into varibale
list(APPEND LIBSBML_EXTRA_LIBS
        ${BZ_LIB}
        ${ICONV_LIB}
        ${LIBSBML2_LIB}
        ${ZDLL}
        )

#MagickWand
set(LINK_WITH_MAGICK OFF CACHE BOOL "Link with Image Magick?")
if (LINK_WITH_MAGICK)
    set(MAGICK_INCLUDE "/usr/include/ImageMagick")
    set(MAGICK_LIBDIR "/usr/lib")
    set(MAGICK_LIBS MagickWand)
endif ()

set(SBNW_DEBUG_LEVEL 2 CACHE STRING "Verbosity of debug checks, must be greater than zero")
set(SBNW_LINK_TO_STATIC_LIBSBML ON CACHE BOOL "Link to a static version of libSBML where available")

if (${WIN32})
    set(SBNW_WIN_COPY_DEPS_TO_INSTALL ON CACHE BOOL "When true, libraries that graphfab depends on (libSBML and its dependencies) will be copied to the graphfab installation directory")
    if (MINGW)
        # Not sure what these are for. Keep them until you test on mingw
        set(SBNW_MINGW ON)
        set(SBNW_NTDDI_VERSION "NTDDI_WIN7" CACHE STRING "Latest Windows version to include support for")
        add_definitions(-DNTDDI_VERSION=${SBNW_NTDDI_VERSION})
    else (MINGW)
        set(SBNW_MINGW OFF)
    endif (MINGW)
    # on windows we need to include Ws2_32.lib for the dll
    list(APPEND LIBSBML_EXTRA_LIBS "Ws2_32.lib")

elseif (${UNIX} OR ${APPLE})

    # set position independent code flag on linux
    set(FPIC_FLAG "-fPIC")

    # turn off windows specific flag
    set(SBNW_WIN_COPY_DEPS_TO_INSTALL OFF CACHE BOOL "Should be off for Linux")

    # enable use of RPATH
    set(ENABLE_FULL_RPATH 1 CACHE BOOL "Enable full RPath in generated executable code")

    # set C/C++ compiler flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline") # -pedantic -Wextra
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif ()

# default spyder support to off
set(SBNW_SPYDER_DIST OFF CACHE BOOL "Install Spyder plugin files")

# default static lib to off
set(BUILD_STATIC_LIB ON CACHE BOOL "Build the static version of the library")

# default python to off
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

# Find libsbml libraries/paths
find_library(LIBSBML_LIBRARY sbml libsbml HINTS ${LIBSBML_PREFIX}/lib)
find_library(LIBSBML_STATIC_LIBRARY sbml-static libsbml-static HINTS ${LIBSBML_PREFIX}/lib)
find_path(LIBSBML_INCLUDE_DIR sbml/SBMLTypes.h PATHS ${LIBSBML_PREFIX}/include)

#Image Magick (not used, remove? )
if (LINK_WITH_MAGICK)
    set(SBNW_USE_MAGICK 1)
    include_directories(${MAGICK_INCLUDE})
else ()
    set(SBNW_USE_MAGICK 0)
endif ()

# RPATH: directories for locating other shared libraries
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

# only allow building of sbnw targets after we have dependencies
if (EXISTS ${LIBSBML_PREFIX})
    message(STATUS "building src")
    add_subdirectory(src)
    #add_subdirectory(sandbox)
    #
    #add_subdirectory(icons)

    #add_subdirectory(testcases)

    if (${BUILD_TESTS})
        add_subdirectory(test)
    endif ()
else ()
    message(WARNING
            "SBNW dependencies have not been found. CMake has been \
            configured to only build sbnw when these dependencies are found. If they are not, however, CMake will\
            dynamically download and build them for you. Therefore, this is how you should proceed:
            1) Build all targets. CMake will go and get googletest, libsbml and its dependencies
            2) Build all targets again. This time around you will have the dependencies and the sbnw targets will be available building.")
endif ()



message(STATUS "
CMAKE Variables
    CMAKE_SOURCE_DIR                        ${CMAKE_SOURCE_DIR}
    CMAKE_BINARY_DIR                        ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_INSTALL_PREFIX                    ${CMAKE_INSTALL_PREFIX}
    CMAKE_BUILD_TYPE                        ${CMAKE_BUILD_TYPE}

Libsbml Variables
    LIBSBML_PREFIX                          ${LIBSBML_PREFIX}
    LIBSBML_LIBRARY                         ${LIBSBML_LIBRARY}
    LIBSBML_STATIC_LIBRARY                  ${LIBSBML_STATIC_LIBRARY}
    LIBSBML_INCLUDE_DIR                     ${LIBSBML_INCLUDE_DIR}
    LIBSBML_EXTRA_LIBS                      ${LIBSBML_EXTRA_LIBS}

Other options
    SBNW_USE_MAGICK                         ${SBNW_USE_MAGICK}
    PYTHON_INCLUDE_DIR                      ${PYTHON_INCLUDE_DIR}
    BUILD_TESTS                             ${BUILD_TESTS}
")

