#include "Hooks.hpp"
#include "../HitboxUtils.hpp"
#include "../Noclip/Hooks.hpp"
#include "HitboxNode.hpp"
#include "ShowHitboxes.hpp"

using namespace geode::prelude;

bool HitboxBaseGameLayer::init()
{
    if (!GJBaseGameLayer::init())
        return false;

    m_fields->node = HitboxNode::create();

    this->addChild(m_fields->node, 10);
    return true;
}

void HitboxBaseGameLayer::resetLevelVariables()
{
    GJBaseGameLayer::resetLevelVariables();

    if (!typeinfo_cast<LevelEditorLayer*>(this))
    {
        if (HitboxTrailResetOnDeath::get()->getRealEnabled())
        {
            m_fields->node->resetTrails();
        }
    }
}

void HitboxEditorLayer::onPlaytest()
{
    LevelEditorLayer::onPlaytest();

    if (HitboxTrailResetOnDeath::get()->getRealEnabled())
    {
        base_cast<HitboxBaseGameLayer*>(this)->m_fields->node->resetTrails();
    }
}


void HitboxEditorUI::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    EditorUI::ccTouchMoved(touch, event);
}

void HitboxEditorUI::transformObjectCall(EditCommand command)
{
    EditorUI::transformObjectCall(command);

    if (auto obj = m_selectedObject)
    {
        moveObject(obj, ccp(0, 0));
    }

    for (auto obj : CCArrayExt<GameObject*>(m_selectedObjects))
    {
        moveObject(obj, ccp(0, 0));
    }
}