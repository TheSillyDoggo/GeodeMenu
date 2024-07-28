cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
	set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(CheatClient VERSION 1.0.0)

file(GLOB SOURCES
	src/*.cpp
	src/*/*.cpp
	src/*/*/*.cpp
)

if ("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS" OR IOS)
    file(
        src/*.mm
        src/*/*.mm
        src/*/*/*.mm
    )
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
include_directories(include)

file(READ "modules.txt" MODULES_CONTENT)

string(REGEX REPLACE "\n$" "" MODULES_CONTENT "${MODULES_CONTENT}")
string(REPLACE "\n" ";" MODULES_LIST "${MODULES_CONTENT}")

foreach(MODULE ${MODULES_LIST})
    if(NOT MODULE MATCHES "^//")
        add_definitions(-D${MODULE})
    endif()
endforeach()

CPMAddPackage("gh:EclipseMenu/RIFT#327c270") # specify a commit!

target_link_libraries(${PROJECT_NAME} rift)

setup_geode_mod(${PROJECT_NAME})
