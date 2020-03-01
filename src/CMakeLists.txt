set(SOURCES
        graphfab/core/SagittariusAssert.c
        graphfab/core/SagittariusCommon.cpp
        graphfab/core/SagittariusException.cpp
        graphfab/error.cpp
        graphfab/draw/tikz.cpp
        graphfab/io/io.cpp
        graphfab/layout.cpp
        graphfab/arrowhead.cpp
        graphfab/box.cpp
        graphfab/canvas.cpp
        graphfab/fr.cpp
        graphfab/point.cpp
        graphfab/math/cubic.cpp
        graphfab/math/geom.cpp
        graphfab/math/transform.cpp
        graphfab/network.cpp
        graphfab/autolayoutSBML.cpp
        graphfab/string.c
        )

set(HEADERS
        graphfab/core/BaseWORDs.h
        graphfab/core/config.h.in
        graphfab/core/Debug.hpp
        graphfab/core/DefaultBool.hpp
        graphfab/core/SagittariusAssert.h
        graphfab/core/SagittariusCommon.h
        graphfab/core/SagittariusConfigure.h
        graphfab/core/SagittariusCore.h
        graphfab/core/SagittariusException.hpp
        graphfab/core/SagittariusPlatform.h
        graphfab/core/SagittariusPlatformWin.h
        graphfab/core/SagittariusPrefetch.h
        graphfab/error.h
        graphfab/draw/magick.h
        graphfab/io/io.h
        graphfab/layout.h
        graphfab/arrowhead.h
        graphfab/box.h
        graphfab/canvas.h
        graphfab/curve.h
        graphfab/fr.h
        graphfab/layoutall.h
        graphfab/point.h
        graphfab/math/allen.h
        graphfab/math/dist.h
        graphfab/math/geom.h
        graphfab/math/min_max.h
        graphfab/math/rand_unif.h
        graphfab/math/round.h
        graphfab/math/sig.h
        graphfab/math/sign_mag.h
        graphfab/math/transform.h
        graphfab/network.h
        graphfab/autolayoutSBML.h
        graphfab/string.h
        )

configure_file(
        graphfab/core/config.h.in
        graphfab/core/config.h
)

##Image Magick-related sources
if (LINK_WITH_MAGICK)
    set(MAGICK_SOURCES
            graphfab/draw/magick.cpp)
    set(MAGICK_HEADERS
            graphfab/draw/magick.h)
else ()
    set(MAGICK_SOURCES) # Clear magick sources
    set(MAGICK_HEADERS)
endif ()

set(SBNW_SOURCES ${SOURCES} ${MAGICK_SOURCES})
set(SBNW_HEADERS ${HEADERS} ${MAGICK_HEADERS})

if (ENABLE_PYTHON_BINDINGS)
    foreach (f ${SOURCES})
        configure_file(${f} ${f} COPYONLY)
    endforeach ()
    foreach (f ${HEADERS})
        configure_file(${f} ${f} COPYONLY)
    endforeach ()
    if (ENABLE_PYTHON_BINDINGS) # FIXME: Why?
        configure_file(graphfab/python/sbnwmodule.c graphfab/python/sbnwmodule.c COPYONLY)
    endif ()
    configure_file(graphfab/setup.py.in setup.py)
endif ()

# Sphinx
configure_file(graphfab/sphinx/source/conf.py graphfab/sphinx/source/conf.py)
configure_file(graphfab/sphinx/source/index.rst graphfab/sphinx/source/index.rst COPYONLY)
configure_file(graphfab/sphinx/Makefile graphfab/sphinx/Makefile COPYONLY)

# ** Shared Library **
add_library(sbnw SHARED ${SBNW_SOURCES})

#include directories
target_include_directories(sbnw PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SBNW_THIRD_PARTY_LIBS}/libsbml/include
        ${SBNW_THIRD_PARTY_LIBS}/libsbml/include/sbml
        ${LIBSBML_INCLUDE_DIR}
        )

##SBML

target_link_libraries(sbnw PRIVATE ${LIBSBML_STATIC_LIBRARY} ${LIBSBML_EXTRA_LIBS})


##Image Magick
if (LINK_WITH_MAGICK)
    target_link_libraries(sbnw ${MAGICK_LIBS})
endif ()

# tell cmake that libsbml is a dependency
add_dependencies(sbnw LIBSBML)

# ** Static Library **
if (BUILD_STATIC_LIB)
    add_library(sbnw_static STATIC ${SBNW_SOURCES})
    target_include_directories(sbnw_static PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${SBNW_THIRD_PARTY_LIBS}/libsbml/include
            ${SBNW_THIRD_PARTY_LIBS}/libsbml/include/sbml
            ${LIBSBML_INCLUDE_DIR}
            )
    target_link_libraries(sbnw_static ${LIBSBML_STATIC_LIBRARY} ${LIBSBML_EXTRA_LIBS})

    # tell cmake that libsbml is a dependency
    add_dependencies(sbnw_static LIBSBML)

    if (${WIN32})
        set_target_properties(sbnw_static PROPERTIES OUTPUT_NAME "libsbnw_static" COMPILE_FLAGS "/DPY_MAJOR_VERSION=${PYTHON_VERSION_MAJOR} /DPYTHON_SCREWED_IT_UP=1" PREFIX "")
    else ()
        set_target_properties(sbnw_static PROPERTIES COMPILE_FLAGS "-fPIC")
    endif ()

    # Image Magick
    if (LINK_WITH_MAGICK)
        target_link_libraries(sbnw_static ${MAGICK_LIBS})
    endif ()
endif ()

#Library dist
install(TARGETS sbnw RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

#Header dist
add_custom_target(headers)
foreach (f ${SBNW_HEADERS})
    add_custom_command(TARGET headers COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${f} ${CMAKE_CURRENT_BINARY_DIR}/../include/sbnw/${f})
endforeach ()

if (${ENABLE_PYTHON})
    add_library(sbnw_pythonmod SHARED graphfab/python/sbnwmodule.c)
    if (BUILD_STATIC_LIB)
        target_link_libraries(sbnw_pythonmod sbnw_static)
    else ()
        target_link_libraries(sbnw_pythonmod sbnw)
    endif ()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        target_link_libraries(sbnw_pythonmod ${PYTHON_LIBRARY})
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set_target_properties(sbnw_pythonmod PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif ()

    set_target_properties(sbnw_pythonmod PROPERTIES OUTPUT_NAME "sbnw" COMPILE_FLAGS "${FPIC_FLAG} -DPYTHON_SCREWED_IT_UP=1" PREFIX "" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sbnw_pythonmod_bin" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sbnw_pythonmod_libn")
    set_property(TARGET sbnw_pythonmod PROPERTY INCLUDE_DIRECTORIES "${PYTHON_INCLUDE_DIR}" APPEND)

    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set_target_properties(sbnw_pythonmod PROPERTIES SUFFIX ".pyd")
    elseif (APPLE)
        set_target_properties(sbnw_pythonmod PROPERTIES SUFFIX ".so")
    endif ()
    install(TARGETS sbnw_pythonmod DESTINATION python/site-packages/sbnw)
    install(FILES graphfab/python/__init__.py DESTINATION python/site-packages/sbnw)
    configure_file(graphfab/python/setup.py.in setup.py)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/setup.py DESTINATION python/site-packages)
endif ()

#Doxygen
#Other files besides headers that contain documentation
set(DOXYGEN_OTHER_FILES)
set(SBNW_DOXY_INPUT "")
foreach (f ${HEADERS} ${DOXYGEN_OTHER_FILES})
    set(SBNW_DOXY_INPUT "${SBNW_DOXY_INPUT} ${CMAKE_CURRENT_SOURCE_DIR}/${f}")
endforeach ()
configure_file(../doxy/doxy.cfg ../doxy/doxy.cfg)
add_custom_target(docs COMMAND doxygen ../doxy/doxy.cfg)


# copy lib to test directory
if (${BUILD_TESTS})
    add_custom_command(TARGET sbnw POST_BUILD        # Adds a post-build event to MyTest
            COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
            "$<TARGET_FILE:sbnw>"      # <--this is in-file
            $<TARGET_FILE_DIR:sbnw_tests>)                 # <--this is out-file path
endif ()