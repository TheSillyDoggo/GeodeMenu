#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/HardStreak.hpp>

class $modify (TrajectoryBGL, GJBaseGameLayer)
{
    virtual bool init();
};

class $modify (TrajectoryEditor, LevelEditorLayer)
{
    bool init(GJGameLevel* level, bool noUI);
};

class $modify (TrajectoryPlayLayer, PlayLayer)
{
    void createObjectsFromSetupFinished();
    
    virtual void destroyPlayer(PlayerObject* player, GameObject* object);
};