#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

class $modify (RangesPLHook, PlayLayer)
{
    bool init(GJGameLevel* level, bool dontCreateObjects);
};

class $modify (RangesBGLHook, GJBaseGameLayer)
{
    void processQueuedButtons(float dt, bool clearInputQueue);
};