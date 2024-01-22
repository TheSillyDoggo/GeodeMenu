#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    bool fix = false;

    virtual void update(float dt)
    {
        PlayerObject::jump();
        GJBaseGameLayer::update(dt);
        
        if (PlayLayer::get() && Client::GetModuleEnabled("instant"))
        {
            if (!m_fields->fix)
            {
                PlayLayer::get()->playPlatformerEndAnimationToPos(PlayLayer::get()->m_player1->m_position, false);
                m_fields->fix = true;
            }
        }
    }
};