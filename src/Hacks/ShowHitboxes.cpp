#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    struct Fields {
        Module* mod = nullptr;
        Module* mod2 = nullptr;
    };

    void updateVisibility(float p0) {
        PlayLayer::updateVisibility(p0);

        if (!m_debugDrawNode)
            return;

        bool shouldVis = GameManager::sharedState()->getGameVariable("0166") && m_isPracticeMode;

        if (!m_fields->mod)
            m_fields->mod = Client::GetModule("show-hitboxes");

        if (!m_fields->mod2)
            m_fields->mod2 = Client::GetModule("show-hitboxes-on-death");

        if (m_fields->mod->enabled)
            shouldVis = true;

        if (m_fields->mod2->enabled && m_player1->m_isDead)
            shouldVis = true;

        if (shouldVis)
            PlayLayer::updateDebugDraw();
        
        m_debugDrawNode->setVisible(shouldVis);
    }

};

#endif