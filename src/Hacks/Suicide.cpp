#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (MenuGameLayer)
{
    struct Fields
    {
        float killDelta;
    };

    virtual void update(float dt)
    {
        MenuGameLayer::update(dt);

        // if (m_playerObject && m_playerObject->getPositionX() > 0) // idk
        m_fields->killDelta += dt;

        if (m_fields->killDelta > 0.3f)
        {
            m_fields->killDelta = 0;

            destroyPlayer();
        }
    }

    QOLMOD_MOD_HOOK("you-should-kill-yourself-now", "MenuGameLayer::update")
};

class $modify (PlayLayer)
{
    virtual void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);

        if (m_player1 && !m_player1->m_isDead)
        {    
            this->PlayLayer::destroyPlayer(m_player1, nullptr);
        }
    }

    QOLMOD_MOD_HOOK("you-should-kill-yourself-now", "PlayLayer::postUpdate")
};