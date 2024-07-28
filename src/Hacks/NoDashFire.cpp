#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (m_dashFireSprite)
            m_dashFireSprite->setVisible(false);
    }

    QOLMOD_MOD_ALL_HOOKS("no-dash-fire")
};