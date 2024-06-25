#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        PlayLayer::destroyPlayer(p0, p1);

        if (auto resetAction = this->getActionByTag(0x10))
        {
            if (auto delayTime = public_cast(as<CCSequence*>(resetAction), m_pActions)[0])
            {
                as<CCDelayTime*>(delayTime)->setDuration(3);
            }
        }
    }
};