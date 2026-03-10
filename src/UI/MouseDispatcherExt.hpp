#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>

using namespace geode::prelude;

#ifndef GEODE_IS_IOS

class $modify (MouseDispatcherExt, CCMouseDispatcher)
{
    static void onModify(auto& self);

    bool handleForNodeRecursive(CCNode* node, float y, float x);

    // hooks
    bool dispatchScrollMSG(float y, float x);
};

#endif