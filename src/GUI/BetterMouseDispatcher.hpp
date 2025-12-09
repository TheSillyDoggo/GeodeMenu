#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>

using namespace geode::prelude;

struct MouseDispatcher
{
    static inline std::vector<CCMouseDelegate*> betterMouseDispatcherDelegates = {};
};

#ifndef GEODE_IS_IOS

class $modify (BetterMouseDispatcher, CCMouseDispatcher)
{
    bool dispatchScrollMSG(float x, float y);
};

#endif