#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        bool v = GameManager::get()->getGameVariable("0026");

        if (PlayLayer::getCurrentPercentInt() > m_level->m_normalPercent.value() && Client::GetModuleEnabled("conditional-retry") && !m_isPracticeMode && !m_player1->m_isPlatformer)
        {
            GameManager::get()->setGameVariable("0026", false);
        }

        PlayLayer::destroyPlayer(p0, p1);

        GameManager::get()->setGameVariable("0026", v);
    }
};