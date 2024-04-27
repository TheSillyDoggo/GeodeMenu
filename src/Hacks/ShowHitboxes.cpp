#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    Module* mod = nullptr;
    Module* mod2 = nullptr;

    CCDrawNode* dn = nullptr;

    CCDrawNode* getNode()
    {
        CCArrayExt<CCNode*> children = this->getChildren();

        for (auto node : children)
        {
            if (typeinfo_cast<CCNode*>(node) && node->getChildrenCount() == 1)
            {
                if (auto l = getChildOfType<CCLayer>(node, 0))
                {
                    if (auto n = getChildOfType<CCDrawNode>(l, 0))
                        return n;
                }
            }
        }

        return nullptr;
    }

    void updateVisibility(float p0) {
        PlayLayer::updateVisibility(p0);

        #ifdef GEODE_IS_WINDOWS
        if (!m_fields->dn)
            m_fields->dn = m_debugDrawNode;
        #endif

        if (!m_fields->dn)
            m_fields->dn = getNode();

        if (!m_fields->dn)
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
        
        m_fields->dn->setVisible(shouldVis);
    }

};

#endif