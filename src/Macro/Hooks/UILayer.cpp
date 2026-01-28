/*#include "UILayer.hpp"
#include "../MacroManager.hpp"
#include <NotificationManager.hpp>

using namespace geode::prelude;

void MacroUILayer::handleKeypress(cocos2d::enumKeyCodes key, bool down, double timestamp)
{
    UILayer::handleKeypress(key, down, timestamp);

    if (MacroManager::get()->getState() == MacroState::Recording)
    {
        if (key == enumKeyCodes::KEY_Up)
        {
            auto sinceLastTick = timestamp - m_gameLayer->m_timestamp;

            MacroManager::get()->storeInput(false, down, m_gameLayer->m_gameState.m_currentProgress, m_gameLayer->m_gameState.m_levelTime, sinceLastTick);
        }
    }

    if (key == enumKeyCodes::KEY_J && down)
    {
        switch (MacroManager::get()->getState())
        {
            case MacroState::None:
                MacroManager::get()->setState(MacroState::Recording);
                NotificationManager::get()->notifyToast("State changed to recording");
                break;

            case MacroState::Recording:
                MacroManager::get()->setState(MacroState::Playback);
                NotificationManager::get()->notifyToast("State changed to playback");
                break;

            case MacroState::Playback:
                MacroManager::get()->setState(MacroState::None);
                NotificationManager::get()->notifyToast("State changed to none");
                break;
        }
    }
}*/