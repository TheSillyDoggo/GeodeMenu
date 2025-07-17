#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

using namespace geode::prelude;

class $modify (SafePlayLayer, PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);

    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle);
    void levelComplete();
    void resetLevel();
};

class $modify (SafeGJGameLevel, GJGameLevel)
{
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4);
};