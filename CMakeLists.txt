cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    add_definitions(-DGLES_SILENCE_DEPRECATION)
    add_definitions(-D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(CheatClient VERSION 1.0.0)

file(GLOB SOURCES
    include/src/*.cpp
    src/*.cpp
	src/*/*.cpp
	src/*/*/*.cpp
	src/*/*/*/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_link_options(/errorlimit:0)

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
include_directories(include src/Client src/Hacks/Speedhack src/Utils src/Notifications src/Localisation src/GUI src/UI/Nodes/Label src/UI)

# CPMAddPackage("gh:TheSillyDoggo/gd-imgui-cocos#2d33a3d") # should be geode

CPMAddPackage("gh:EclipseMenu/RIFT#v1")
CPMAddPackage("gh:rapidfuzz/rapidfuzz-cpp#main")
CPMAddPackage("gh:maxnut/GDReplayFormat#72078e5")

target_link_libraries(${PROJECT_NAME} rift rapidfuzz::rapidfuzz libGDR)

setup_geode_mod(${PROJECT_NAME})