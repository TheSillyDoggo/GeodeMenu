#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    void switchedToMode(GameObjectType p0)
    {
        auto ball = m_isBall;

        PlayerObject::switchedToMode(p0);

        if (ball && !m_isBall)
        {
            this->runAction(CCEaseOut::create(CCRotateBy::create(0.5f, 360 - this->getRotation()), 2));
        }
    }

    QOLMOD_MOD_ALL_HOOKS("ball-rotation-bug")
};