#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/UILayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "LabelLayer.hpp"

using namespace geode::prelude;

class $modify (LabelsUILayer, UILayer)
{
    struct Fields
    {
        LabelLayer* labelLayer;
    };

    bool init(GJBaseGameLayer* bgl);
};

class $modify (LabelBaseGameLayer, GJBaseGameLayer)
{
    void resetLevelVariables();
};