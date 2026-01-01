#pragma once

#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

enum class NoclipPlayerSelector
{
    All,
    Player1,
    Player2,
};

class $modify (NoclipBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        bool hasDiedThisAttempt = false;
        int timeInLevel = 0;

        bool hasDiedThisTick = false;
        bool didDieLastTick = false;

        bool hasP1DiedThisTick = false;
        bool didP1DieLastTick = false;
        bool hasP2DiedThisTick = false;
        bool didP2DieLastTick = false;

        int totalTicksDead = 0;
        int p1TicksDead = 0;
        int p2TicksDead = 0;
        
        int totalDeaths = 0;
        int p1Deaths = 0;
        int p2Deaths = 0;

        GameObject* deathObject;
    };

    float getNoclipAccuracy(NoclipPlayerSelector selector);
    int getNoclipDeaths(NoclipPlayerSelector selector);

    void playerDied(NoclipPlayerSelector selector);
    void onPostUpdate();

    GameObject* getDeathObject();

    bool shouldPlayerRegularDie(PlayerObject* pl, GameObject* go);

    void resetNoclipValues();
};