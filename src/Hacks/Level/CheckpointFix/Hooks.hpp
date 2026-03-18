#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayerCheckpoint.hpp>
#include "PlayerState.hpp"

class $modify (FixedPlayerCheckpoint, PlayerCheckpoint)
{
    struct Fields
    {
        qolmod::PlayerState state = {};
    };
};

class $modify (FixedPlayerObject, PlayerObject)
{
    void loadFromCheckpoint(PlayerCheckpoint* checkpoint);
    void saveToCheckpoint(PlayerCheckpoint* checkpoint);
};