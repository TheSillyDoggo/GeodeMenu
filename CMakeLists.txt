cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    add_definitions(-DGLES_SILENCE_DEPRECATION)
else()
	set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(CheatClient VERSION 1.0.0)

file(GLOB SOURCES
	src/*.cpp
	src/*/*.cpp
	src/*/*/*.cpp
)

#if ("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS" OR IOS)
#    src/*.mm
#    src/*/*.mm
#    src/*/*/*.mm
#endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Windows specific settings
if (WIN32)
  # Disable annoying warnings
  target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)

  # Export all symbols for better debugging
  set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

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

setup_geode_mod(${PROJECT_NAME})
