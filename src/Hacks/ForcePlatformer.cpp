#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        if (Client::GetModuleEnabled("force-plat"))
        {
            this->togglePlatformerMode(true);
            #ifdef GEODE_IS_ANDROID
            PlayLayer::get()->m_uiLayer->togglePlatformerMode(true);
            #endif
        }

        PlayerObject::update(dt);
    }
};