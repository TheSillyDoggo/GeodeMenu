#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/MenuLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (MenuLayer)
{
    virtual bool init()
    {
        if (!MenuLayer::init())
            return false;

        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);

        if (localTime->tm_mon + 1 == 12)
        {
            auto snow = CCParticleSnow::create();
            snow->setID("snow"_spr);

            this->addChild(snow, 420);
        }

        return true;
    }

    QOLMOD_MOD_HOOK("december-snow", "MenuLayer::init")
};