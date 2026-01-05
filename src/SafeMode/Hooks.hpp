#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class $modify (SafePlayLayer, PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);

    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle);
    void levelComplete();
    void resetLevel();
    
    virtual void postUpdate(float dt);
};

class $modify (SafeGJGameLevel, GJGameLevel)
{
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4);

    static void onModify(auto& self);
};

class $modify (SafeEndLevelLayer, EndLevelLayer)
{
    struct Fields
    {
        CCNode* ci = nullptr;
    };

    virtual void customSetup();

    void updateCheatIndicator(float);
    void onViewReasons(CCObject* sender);
    
    void playCoinEffect(float duration);
};