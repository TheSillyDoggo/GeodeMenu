#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* jumpHack = nullptr;

class $modify (GJBaseGameLayer)
{
    virtual void update(float dt)
    {
        if (!jumpHack)
            jumpHack = Client::GetModule("jump-hack");

        if (jumpHack->enabled)
            m_player1->m_isOnGround = true;

        GJBaseGameLayer::update(dt);
        
        if (jumpHack->enabled)
            m_player1->m_isOnGround = true;
    }
};