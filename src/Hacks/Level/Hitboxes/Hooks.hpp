#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "HitboxNode.hpp"

class $modify (HitboxBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        HitboxNode* node = nullptr;
        CCNode* nodeContainer = nullptr;
    };

    void onTick(bool updateTrail);

    void resetLevelVariables();
    virtual bool init();
};

class $modify (HitboxPlayLayer, PlayLayer)
{
    virtual void postUpdate(float dt);
};

class $modify (HitboxEditorLayer, LevelEditorLayer)
{
    void onPlaytest();

    virtual void postUpdate(float dt);
    virtual void updateVisibility(float dt);
};