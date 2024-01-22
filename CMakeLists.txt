cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(CheatClient VERSION 1.0.0)

file(GLOB SOURCES
	src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES}  "src/Client/Module.h" "src/Client/Window.h" "src/Client/Client.h" "src/Client/DrawUtils.h" "src/Client/ColourUtility.cpp" "src/Client/ClientSetup.h" "src/Hacks/UnlockHacks.cpp" "src/Hacks/CloseEndLevelLayer.cpp" "src/Hacks/TransitionTimeCustomiser.cpp" "src/Hacks/Speedhack.cpp" "src/Hacks/Noclip.cpp" "src/Hacks/Hitbox.cpp" "src/Hacks/NoRotation.cpp" "src/Hacks/Variables.cpp" "src/Hacks/FullOptions.cpp" "src/Hacks/StatusText.cpp" "src/Hacks/FireInTheHole.cpp" "src/Hacks/InstantComplete.cpp")

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

CPMAddPackage("gh:matcool/gd-imgui-cocos#4eb5777") # specify a commit!

target_link_libraries(${PROJECT_NAME} imgui-cocos)

setup_geode_mod(${PROJECT_NAME})
