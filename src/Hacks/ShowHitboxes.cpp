#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

CCNode* getChildByTagRecursively(CCNode* parent, int tag) {
    if (!parent) {
        return nullptr;
    }

    if (parent->getTag() == tag) {
        return parent;
    }

    CCArray* children = parent->getChildren();
    CCObject* obj = nullptr;
    CCARRAY_FOREACH(children, obj) {
        CCNode* child = dynamic_cast<CCNode*>(obj);
        if (child) {
            CCNode* result = getChildByTagRecursively(child, tag);
            if (result && typeinfo_cast<CCDrawNode*>(result)) {
                return result;
            }
        }
    }

    return nullptr;
}

class $modify (PlayLayer)
{
    Module* mod = nullptr;
    Module* mod2 = nullptr;

    CCDrawNode* dn = nullptr;

    void updateVisibility(float p0) {
        PlayLayer::updateVisibility(p0);

        if (!m_fields->dn)
            m_fields->dn = as<CCDrawNode*>(getChildByTagRecursively(PlayLayer::get(), -9999));

        if (!m_fields->dn)
            m_fields->dn = as<CCDrawNode*>(getChildByTagRecursively(PlayLayer::get(), 1));

        if (!m_fields->dn && PlayLayer::get()->getChildByID("hitbox-node"))
            m_fields->dn = as<CCDrawNode*>(getChildOfType<CCDrawNode>(getChildOfType<CCLayer>(PlayLayer::get()->getChildByID("hitbox-node"), 0), 0));

        if (!m_fields->dn)
            return;

        #ifdef GEODE_IS_WINDOWS
        bool shouldVis = m_isDebugDrawEnabled && m_isPracticeMode;
        #else
        //bool shouldVis = m_isPracticeMode;
        bool shouldVis = false;
        #endif

        if (!m_fields->mod)
            m_fields->mod = Client::GetModule("show-hitboxes");

        if (!m_fields->mod2)
            m_fields->mod2 = Client::GetModule("show-hitboxes-on-death");

        if (m_fields->mod->enabled)
            shouldVis = true;

        if (m_fields->mod2->enabled && m_player1->m_isDead)
            shouldVis = true;

        PlayLayer::updateDebugDraw();
        m_fields->dn->setVisible(shouldVis);
    }

};