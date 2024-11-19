#ifndef GEODE_IS_ANDROID

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/UILayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "LabelLayer.hpp"

using namespace geode::prelude;

class $modify (LabelsUILayer, UILayer)
{
    struct Fields
    {
        LabelLayer* labelLayer;
    };

    void regenerateLabels();

    bool init(GJBaseGameLayer* bgl);
};

class $modify (LabelBaseGameLayer, GJBaseGameLayer)
{
    void resetLevelVariables();

    void handleButton(bool down, int button, bool isPlayer1);
};

class $modify (LabelPlayLayer, PlayLayer)
{
    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1);
};

#endif