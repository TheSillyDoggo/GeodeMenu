#include <Geode/Geode.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (UILayer)
{
    void onPause(cocos2d::CCObject* sender)
    {
        m_gameLayer->m_gameState.m_pauseCounter = 0;

        UILayer::onPause(sender);
    }

    void handleKeypress(cocos2d::enumKeyCodes p0, bool p1)
    {
        m_gameLayer->m_gameState.m_pauseCounter = 0;

        UILayer::handleKeypress(p0, p1);
    }

    QOLMOD_MOD_ALL_HOOKS("pause-buffering")
};