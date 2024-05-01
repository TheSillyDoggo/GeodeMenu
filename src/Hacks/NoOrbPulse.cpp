#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCCircleWave.hpp>
#include "../Client/Client.h"
#include "../Utils/Utils.hpp"

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

/*class $modify (PlayLayer)
{
    virtual void updateVisibility(float p0)
    {
        log::info("v: {}", MBO(float, FMODAudioEngine::sharedEngine(), 0x178));
        MBO(float, FMODAudioEngine::sharedEngine(), 0x178) = 1.0f;
        log::info("v2: {}", MBO(float, FMODAudioEngine::sharedEngine(), 0x178));
        ///**(reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(FMODAudioEngine::sharedEngine()) + 0x178)) = 1.0f;

        log::info("asdf");

        PlayLayer::updateVisibility(p0);
    }
};*/