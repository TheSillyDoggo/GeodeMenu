#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

class $modify (LabelPlayLayer, PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);

    virtual void resetLevel();
};

class $modify (LabelBaseGameLayer, GJBaseGameLayer)
{
    void handleButton(bool down, int button, bool isPlayer1, double timestamp);
};