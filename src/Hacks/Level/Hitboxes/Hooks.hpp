#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GJGameState.hpp>
#include "HitboxNode.hpp"

class $modify (HitboxBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        HitboxNode* node = nullptr;
        CCNode* nodeContainer = nullptr;
    };

    void onTick(bool updateTrail);

    // checkRepellPlayer runs right before updateCamera, so i use that to check if its the call at the end of a tick
    void checkRepellPlayer();
    void updateCamera(float dt);

    void resetLevelVariables();
    virtual bool init();
};

class $modify (HitboxPlayLayer, PlayLayer)
{
    
};

class $modify (HitboxEditorLayer, LevelEditorLayer)
{
    void onPlaytest();

    virtual void updateVisibility(float dt);
};