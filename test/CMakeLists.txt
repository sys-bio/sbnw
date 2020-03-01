#enable_testing()


#add_subdirectory(
#        "${CMAKE_BINARY_DIR}/googletest-src"
#        "${CMAKE_BINARY_DIR}/googletest-build"
#        )
#find_package(GTest REQUIRED PATHS ${GOOGLETEST_PREFIX}/lib/cmake)
#
#message(STATUS "GTest_FOUND ${GTest_FOUND}")
#message(STATUS "Gtest_LIBRARIES ${Gtest_LIBRARIES}")
#message(STATUS "GTEST_MAIN_LIBRARIES ${Gtest_MAIN_LIBRARIES}")
#message(STATUS "GTEST_LIBRARY" ${GTEST_LIBRARY})

add_executable(sbnw_tests FakeTest.cpp )

# set target to statically link
set_property(TARGET sbnw_tests PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_dependencies(sbnw_tests googletest)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
# https://crascit.com/2015/07/25/cmake-gtest/
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

target_include_directories(sbnw_tests PRIVATE
        ${GOOGLETEST_PREFIX}/include
        )

message(STATUS "CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")
target_link_libraries(sbnw_tests PRIVATE
#        "${CMAKE_BINARY_DIR}/src/libsbnw_static.lib"
        ${GOOGLETEST_PREFIX}/lib/gtest.lib
        ${GOOGLETEST_PREFIX}/lib/gtest_main.lib
        )







