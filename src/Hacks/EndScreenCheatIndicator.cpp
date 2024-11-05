#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include "../Client/Client.h"
#include "SafeMode/SafeMode.hpp"

using namespace geode::prelude;

class $modify (EndLevelLayer)
{
    virtual void customSetup()
    {
        EndLevelLayer::customSetup();

        auto ind = CCSprite::createWithSpriteFrameName("uiDot_001.png");
        ind->setColor(SafeMode::get()->colourForState());
        ind->setOpacity(150);
        ind->setScale(1.5f);
        ind->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(-167, 100));

        m_mainLayer->addChild(ind, 42069);
    }

    QOLMOD_MOD_HOOK("end-screen-cheat-indicator", "EndLevelLayer::customSetup")
};