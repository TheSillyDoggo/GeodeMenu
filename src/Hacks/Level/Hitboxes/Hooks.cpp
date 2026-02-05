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

    m_fields->nodeContainer = CCNode::create();
    m_fields->nodeContainer->setZOrder(10);

    m_fields->node = HitboxNode::create();
    m_fields->nodeContainer->addChild(m_fields->node);

    this->addChild(m_fields->nodeContainer);
    return true;
}

void HitboxBaseGameLayer::onTick(bool updateTrail)
{
    auto fields = m_fields.self();
    fields->node->setVisible(HitboxUtils::shouldHitboxesBeVisible() || (ShowHitboxesOnDeath::get()->getRealEnabled() ? m_player1->m_isDead : false) || ShowHitboxes::get()->getRealEnabled());

    if (fields->node->isVisible())
    {
        auto dd = m_isDebugDrawEnabled;
        m_debugDrawNode->setVisible(m_isEditor);

        auto copy = m_debugDrawNode->getParent();
        fields->nodeContainer->setPosition(CCDirector::get()->getWinSize() / 2);
        fields->node->setPosition(copy->getPosition() - fields->nodeContainer->getPosition());
        fields->node->setScale(copy->getScale());
        fields->nodeContainer->setRotation(m_gameState.m_cameraAngle);

        if (ShowHitboxesOnDeathDeathObjOnly::get()->getRealEnabled() && (m_player1->m_isDead || (m_player2 && m_player2->m_isDead)))
        {
            if (ShowHitboxes::get()->getRealEnabled() || (m_isDebugDrawEnabled && m_isPracticeMode))
                fields->node->setOnlyObject(nullptr);
            else
                fields->node->setOnlyObject(base_cast<NoclipBaseGameLayer*>(this)->getDeathObject());
        }
        else
            fields->node->setOnlyObject(nullptr);

        fields->node->updateNode();
    }

    if (updateTrail)
    {
        if (HitboxTrail::get()->getRealEnabled() && (fields->node->isVisible() ? true : ShowHitboxesOnDeathTrail::get()->getRealEnabled()))
        {
            if (!m_levelEndAnimationStarted)
            {
                if (!m_player1->m_isDead)
                    fields->node->storePlayerTrail(m_player1);

                if (m_player2 && m_player2->isRunning())
                {
                    if (!m_player2->m_isDead)
                        fields->node->storePlayerTrail(m_player2);
                }
            }
        }
    }
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

void HitboxEditorLayer::updateVisibility(float dt)
{
    LevelEditorLayer::updateVisibility(dt);

    base_cast<HitboxBaseGameLayer*>(this)->onTick(false);
}

void HitboxPlayLayer::postUpdate(float dt)
{
    PlayLayer::postUpdate(dt);

    if (!m_started)
        base_cast<HitboxBaseGameLayer*>(this)->onTick(false);
}

bool isEndTickCameraUpdate = false;

void HitboxBaseGameLayer::checkRepellPlayer()
{
    GJBaseGameLayer::checkRepellPlayer();

    isEndTickCameraUpdate = true;
}

void HitboxBaseGameLayer::updateCamera(float dt)
{
    GJBaseGameLayer::updateCamera(dt);

    if (isEndTickCameraUpdate)
    {
        onTick(true);
    }
    isEndTickCameraUpdate = false;
}