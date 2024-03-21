#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCCircleWave.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* orbPulse = nullptr;

class $modify (CCCircleWave)
{
    virtual void draw()
    {
        if (!orbPulse)
            orbPulse = Client::GetModule("no-circles");

        if (!PlayLayer::get() || !orbPulse->enabled) // if it isn't enabled or it isn't the level
            CCCircleWave::draw();
    }
};