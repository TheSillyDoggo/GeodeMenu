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

#add_library(${PROJECT_NAME} SHARED ${SOURCES}  "src/Client/Module.h" "src/Client/Window.h" "src/Client/Client.h" "src/Client/DrawUtils.h" "src/Client/ColourUtility.cpp" "src/Client/ClientSetup.h" "src/Hacks/UnlockHacks.cpp" "src/Hacks/CloseEndLevelLayer.cpp" "src/Hacks/TransitionTimeCustomiser.cpp" "src/Hacks/Speedhack.cpp" "src/Hacks/Noclip.cpp" "src/Hacks/Hitbox.cpp" "src/Hacks/NoRotation.cpp" "src/Hacks/Variables.cpp" "src/Hacks/FullOptions.cpp" "src/Hacks/StatusText.cpp" "src/Hacks/FireInTheHole.cpp" "src/Hacks/InstantComplete.cpp" "src/Hacks/ForcePlatformer.cpp" "src/Hacks/CopyHack.cpp" "src/Hacks/NocMark.cpp" "src/Hacks/SafeMode.cpp" "src/Hacks/SliderLimit.cpp" "src/Hacks/PracticeAlert.cpp" "src/Hacks/TextLimitBypass.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

file(READ "modules.txt" MODULES_CONTENT)

string(REGEX REPLACE "\n$" "" MODULES_CONTENT "${MODULES_CONTENT}")
string(REPLACE "\n" ";" MODULES_LIST "${MODULES_CONTENT}")

foreach(MODULE ${MODULES_LIST})
    if(NOT MODULE MATCHES "^//")
        add_definitions(-D${MODULE})
    endif()
endforeach()

setup_geode_mod(${PROJECT_NAME})
