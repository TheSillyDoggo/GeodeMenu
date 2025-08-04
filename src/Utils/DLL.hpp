#pragma once

#include <Geode/Geode.hpp>

#define QOLMOD_EXPORTING

#ifdef GEODE_IS_WINDOWS
    #ifdef QOLMOD_EXPORTING
        #define DLL __declspec(dllexport)
    #else
        #define DLL __declspec(dllimport)
    #endif
#else
    #define DLL __attribute__((visibility("default")))
#endif