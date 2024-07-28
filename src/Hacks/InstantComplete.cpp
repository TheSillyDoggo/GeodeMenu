#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    struct Fields {
        bool fix = false;
    };

    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);
        
        if (typeinfo_cast<PlayLayer*>(this))
        {
            if (!m_fields->fix)
            {
                typeinfo_cast<PlayLayer*>(this)->playPlatformerEndAnimationToPos(ccp(2, 2), false);
                m_fields->fix = true;
            }
        }
    }

    QOLMOD_MOD_ALL_HOOKS("instant")
};