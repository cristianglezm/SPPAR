cmake_minimum_required(VERSION 3.1)

################################################################################
### User options.
################################################################################

option(SPPAR_BUILD_TESTS "Build Tests" OFF)
option(SPPAR_BUILD_EXAMPLES "Build Examples" OFF)
option(SPPAR_BUILD_STATIC "Build static" OFF)
option(SPPAR_BUILD_DOCS "Build documentation" OFF)

if(SPPAR_BUILD_STATIC)
    SET(BUILD_SHARED_LIBS FALSE)
endif(SPPAR_BUILD_STATIC)

################################################################################
### Project files.
################################################################################

project(SPPAR)

SET(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/include")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

FILE(GLOB_RECURSE SPPAR_INCLUDES "include/*.hpp")

#FILE(GLOB_RECURSE SPPAR_SOURCES "src/*.cpp")
#if(NOT SPPAR_BUILD_STATIC)
#	add_library(SPPAR SHARED ${SPPAR_SOURCES} ${SPPAR_INCLUDES})
#else()
#	add_library(SPPAR STATIC ${SPPAR_SOURCES} ${SPPAR_INCLUDES})
#endif(NOT SPPAR_BUILD_STATIC)

############
# header only
############

add_library(HEADER_ONLY_TARGET STATIC ${SPPAR_INCLUDES})
set_target_properties(HEADER_ONLY_TARGET PROPERTIES LINKER_LANGUAGE CXX)

set(SPPAR_CONFIG_HPP_INPUT "${CMAKE_SOURCE_DIR}/include/SPPAR/Config.hpp")
FILE(READ "${SPPAR_CONFIG_HPP_INPUT}" SPPAR_CONFIG_HPP_CONTENTS)
STRING(REGEX REPLACE ".*#define SPPAR_VERSION_MAJOR ([0-9]+).*" "\\1" SPPAR_VERSION_MAJOR "${SPPAR_CONFIG_HPP_CONTENTS}")
STRING(REGEX REPLACE ".*#define SPPAR_VERSION_MINOR ([0-9]+).*" "\\1" SPPAR_VERSION_MINOR "${SPPAR_CONFIG_HPP_CONTENTS}")
STRING(REGEX REPLACE ".*#define SPPAR_VERSION_PATCH ([0-9]+).*" "\\1" SPPAR_VERSION_PATCH "${SPPAR_CONFIG_HPP_CONTENTS}")
set(SPPAR_VERSION ${SPPAR_VERSION_MAJOR}.${SPPAR_VERSION_MINOR}.${SPPAR_VERSION_PATCH})

#set_target_properties(SPPAR PROPERTIES VERSION ${SPPAR_VERSION} SOVERSION ${SPPAR_VERSION_MAJOR})
#set_target_properties(sPPAR PROPERTIES OUTPUT_NAME "SPPAR-${SPPAR_VERSION_MAJOR}")

################################################################################
### Dependencies.
################################################################################

if(SPPAR_BUILD_TESTS)
	find_package(GTest REQUIRED)
	find_package(GMock REQUIRED)
endif(SPPAR_BUILD_TESTS)

################################################################################
### Enable C++14 and warnings
################################################################################

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX "_d")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fexpensive-optimizations")
endif()

################################################################################
### Testing
################################################################################

if(SPPAR_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif(SPPAR_BUILD_TESTS)

################################################################################
### examples
################################################################################

if(SPPAR_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif(SPPAR_BUILD_EXAMPLES)

#################################################################################
### Docs
#################################################################################

if(SPPAR_BUILD_DOCS)
    find_package(Doxygen)
    IF(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in 
			${CMAKE_CURRENT_BINARY_DIR}/doxyfile @ONLY)
        SET(DOXYGEN_INPUT "${CMAKE_CURRENT_BINARY_DIR}/doxyfile")
        SET(DOXYGEN_OUTPUT "${CMAKE_SOURCE_DIR}/docs")

        ADD_CUSTOM_COMMAND(
          OUTPUT ${DOXYGEN_OUTPUT}
          COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
          COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
          COMMAND ${CMAKE_COMMAND} -E echo "Done."
          DEPENDS ${DOXYGEN_INPUT})

        ADD_CUSTOM_TARGET(docs ALL DEPENDS ${DOXYGEN_OUTPUT})

        ADD_CUSTOM_TARGET(docs_forced
          COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
          COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
          COMMAND ${CMAKE_COMMAND} -E echo "Done.")
    ENDIF(DOXYGEN_FOUND)
endif(SPPAR_BUILD_DOCS)

#################################################################################
### Installing Library
#################################################################################

#install(TARGETS SPPAR LIBRARY DESTINATION bin RUNTIME DESTINATION bin
#						ARCHIVE DESTINATION lib)
install(FILES LICENSE.txt DESTINATION .)
install(FILES README.md DESTINATION .)
if(SPPAR_BUILD_DOCS)
    install(DIRECTORY docs DESTINATION .)
endif(SPPAR_BUILD_DOCS)
install(DIRECTORY include DESTINATION .)
