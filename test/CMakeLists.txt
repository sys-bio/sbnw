add_executable(sbnw_tests sbnwTests.cpp SBNWClientAPI.h UniformRNGTests.cpp)
set_property(TARGET sbnw_tests PROPERTY CXX_STANDARD ${CXX_STANDARD})

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
        ${CMAKE_SOURCE_DIR}/src
        ${LIBSBML_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

message(STATUS "CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")
target_link_libraries(sbnw_tests PRIVATE
#        $<TARGET_FILE:sbnw>
        ${GOOGLETEST_PREFIX}/lib/gtest.lib
        ${GOOGLETEST_PREFIX}/lib/gtest_main.lib
        )

add_definitions(-DTEST_DIRECTORY=${CMAKE_CURRENT_SOURCE_DIR})






