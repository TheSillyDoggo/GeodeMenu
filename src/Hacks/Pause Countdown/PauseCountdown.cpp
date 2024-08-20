#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/UILayer.hpp>
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

class $modify (UILayer)
{
    void onPause(cocos2d::CCObject* sender)
    {
        if (CountdownLayer::get())
            return CountdownLayer::get()->keyBackClicked();

        UILayer::onPause(sender);
    }
};