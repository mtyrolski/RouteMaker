cmake_minimum_required(VERSION 3.0)
project(Drogi C)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra")
# set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
# set(CMAKE_C_FLAGS_DEBUG "-g")

set(SOURCE_FILES
        src/map_main.c
        src/utils.h
        src/utils.c
        src/map.h
        src/map.c
        src/road.h
        src/road.c
        src/city.h
        src/route.h
        src/route.c
        src/city.c
        src/queue.h
        src/queue.c
        src/int_list.h
        src/int_list.c
        src/input_manager.h
        src/input_manager.c
        src/string_vector.h
        src/string_vector.c
        src/avl_tree.h
        src/avl_tree.c)

add_executable(map ${SOURCE_FILES})

find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
	    )
endif (DOXYGEN_FOUND)
