#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

class $modify (TrajectoryBGL, GJBaseGameLayer)
{

};

class $modify (TrajectoryPlayLayer, PlayLayer)
{
    struct Fields
    {
        cocos2d::CCDrawNode* trajectoryDrawNode = nullptr;
        PlayerObject* simulationPlayer = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    void createObjectsFromSetupFinished();
    virtual void postUpdate(float dt);
    virtual void destroyPlayer(PlayerObject* player, GameObject* object);

    void updateSimulation();
    void simulatePlayer(PlayerObject* player);
};