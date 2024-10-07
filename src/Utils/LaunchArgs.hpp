#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class LaunchArgs
{
    private:
        static inline LaunchArgs* instance;
    public:
        static LaunchArgs* get();

        bool hasLaunchArg(std::string arg);
};