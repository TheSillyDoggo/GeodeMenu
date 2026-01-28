/*#include "GJBaseGameLayer.hpp"
#include "../MacroManager.hpp"

using namespace geode::prelude;

void MacroBaseGameLayer::resetLevelVariables()
{
    GJBaseGameLayer::resetLevelVariables();

    MacroManager::get()->markAllUnplayed();

    if (MacroManager::get()->getState() == MacroState::Recording)
    {
        MacroManager::get()->clearInputs();
    }
}

void MacroBaseGameLayer::processCommands(float dt)
{
    if (MacroManager::get()->getState() == MacroState::Playback)
    {
        for (auto input : MacroManager::get()->getUnplayedInputs())
        {
            double game = m_gameState.m_levelTime + dt;
            double macro = input.levelTime + input.cbfOffset;

            if (game >= macro)
            {
                MacroManager::get()->markPlayed(input.uniqueID, true);

                dt -= (game - macro);
                m_uiLayer->handleKeypress(enumKeyCodes::KEY_Up, input.down, m_timestamp + input.cbfOffset);
            }
        }
    }

    GJBaseGameLayer::processCommands(dt);
}*/