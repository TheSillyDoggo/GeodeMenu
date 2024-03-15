#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* instant = nullptr;

class $modify (GJBaseGameLayer)
{
    bool fix = false;

    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);

        if (!instant)
            instant = Client::GetModule("instant");
        
        if (PlayLayer::get() && instant->enabled)
        {
            if (!m_fields->fix)
            {
                PlayLayer::get()->playPlatformerEndAnimationToPos(ccp(2, 2), false);
                m_fields->fix = true;
            }
        }
    }
};

#endif