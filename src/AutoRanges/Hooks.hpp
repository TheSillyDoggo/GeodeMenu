#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

class $modify (RangesBGLHook, GJBaseGameLayer)
{
    virtual bool init();
    void processQueuedButtons(float dt, bool clearInputQueue);
};