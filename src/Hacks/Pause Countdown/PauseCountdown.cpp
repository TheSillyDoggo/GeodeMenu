#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../../Client/Client.h"
#include "CountdownLayer.hpp"

using namespace geode::prelude;

class $modify (PauseLayer)
{
    void onResume(cocos2d::CCObject* sender)
    {
        auto countdown = CountdownLayer::create();
        CCScene::get()->addChild(countdown);

        CCTouchDispatcher::get()->unregisterForcePrio(this);
        this->removeFromParent();
    }

    QOLMOD_MOD_ALL_HOOKS("pause-countdown")
};