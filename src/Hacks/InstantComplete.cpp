#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    bool fix = false;

    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);
        
        if (PlayLayer::get() && Client::GetModuleEnabled("instant"))
        {
            if (!m_fields->fix)
            {
                PlayLayer::get()->playPlatformerEndAnimationToPos(ccp(0, 0), false);
                m_fields->fix = true;
            }
        }
    }
};