#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/DashRingObject.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* platMod = nullptr;

#ifdef GEODE_IS_ANDROID

class $modify (UILayer)
{
    bool init(GJBaseGameLayer* p0)
    {
        if (!UILayer::init(p0))
            return false;

        if (!platMod)
            platMod = Client::GetModule("force-plat");

        if (platMod->enabled)
            togglePlatformerMode(true);

        return true;
    }
};

#endif

class $modify (DashRingObject)
{

};

class $modify (PlayerObject)
{
    bool f;

    virtual void update(float dt)
    {
        if (!platMod)
            platMod = Client::GetModule("force-plat");

        if (platMod->enabled)
        {

            this->togglePlatformerMode(true);
            #ifdef GEODE_IS_ANDROID
            //if (!m_fields->f)
            //{
            //    m_fields->f = true;
            //    if (PlayLayer::get() && PlayLayer::get()->m_uiLayer)
            //        PlayLayer::get()->m_uiLayer->togglePlatformerMode(true);
            //}
            #endif

        }

        PlayerObject::update(dt);
    }
};