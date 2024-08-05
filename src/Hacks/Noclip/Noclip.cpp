#include "Noclip.hpp"
#include "../SafeMode/SafeMode.hpp"

bool NoclipPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    m_fields->tintOnDeath = Client::GetModule("noclip")->options[0];
    m_fields->tintOpacity = as<SliderModule*>(Client::GetModule("noclip")->options[1]);

    m_fields->tint = CCLayerColor::create(ccc4(255, 0, 0, 0));
    m_fields->tint->setID("noclip-tint-popup"_spr);
    m_fields->tint->setZOrder(1);

    this->addChild(m_fields->tint);

    return true;
}

void NoclipPlayLayer::destroyPlayer(PlayerObject* p0, GameObject* p1)
{
    if (p0 && p0->getID() == "trajectory-player"_spr)
    {
        p0->m_isDead = true;

        return PlayLayer::destroyPlayer(p0, p1);
    }

    if (!base_cast<NoclipBaseGameLayer*>(this)->m_fields->ac)
        base_cast<NoclipBaseGameLayer*>(this)->m_fields->ac = p1;

    if (!Client::GetModuleEnabled("noclip") || (base_cast<NoclipBaseGameLayer*>(this)->m_fields->ac == p1))
        PlayLayer::destroyPlayer(p0, p1);
    else
    {
        SafeMode::get()->setHackedAttempt();

        if (!base_cast<NoclipBaseGameLayer*>(this)->m_fields->hasDied)
        {
            base_cast<NoclipBaseGameLayer*>(this)->m_fields->hasDied = true;
            base_cast<NoclipBaseGameLayer*>(this)->m_fields->timeDead += CCDirector::get()->getDeltaTime();
        }
        
        m_fields->isDead = true;

        if (m_fields->last != p1)
        {
            m_fields->last = p1;
            
            m_fields->d++;
        }

        if (base_cast<NoclipBaseGameLayer*>(this)->m_fields->ac != p1)
            m_fields->t++;

        if (m_fields->tintOnDeath->enabled)
        {
            m_fields->tint->stopAllActions();
            m_fields->tint->setOpacity(m_fields->tintOpacity->value * 255);
            m_fields->tint->runAction(CCFadeTo::create(0.35f, 0));
        }
    }
}

void NoclipPlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    m_fields->d = 0;
    m_fields->t = 0;
    base_cast<NoclipBaseGameLayer*>(this)->m_fields->timeDead = 0;
    m_fields->isDead = false;
}

void NoclipBaseGameLayer::update(float dt)
{
    GJBaseGameLayer::update(dt);

    m_fields->hasDied = false;
}

void NoclipBaseGameLayer::resetLevelVariables()
{
    GJBaseGameLayer::resetLevelVariables();

    m_fields->timeDead = 0;
}

float NoclipBaseGameLayer::getNoclipAccuracy()
{
    if (m_gameState.m_currentProgress == 0)
        return 1;

    return 1 - (m_fields->timeDead / as<float>(m_gameState.m_levelTime));
}

void NoclipEditorLayer::playerTookDamage(PlayerObject* p0)
{
    if (Client::GetModuleEnabled("noclip"))
    {
        auto nbgl = base_cast<NoclipBaseGameLayer*>(this);

        if (!nbgl->m_fields->ac)
            nbgl->m_fields->ac = p0->m_collidedObject;

        if (p0 == nbgl->m_fields->ac)
            return LevelEditorLayer::playerTookDamage(p0);

        if (!nbgl->m_fields->hasDied)
        {
            nbgl->m_fields->hasDied = true;
            nbgl->m_fields->timeDead += CCDirector::get()->getDeltaTime();
        }

        SafeMode::get()->setHackedAttempt();
        return;
    }

    LevelEditorLayer::playerTookDamage(p0);
}

void NoclipEditorLayer::postUpdate(float p0)
{
    if (Client::GetModuleEnabled("noclip"))
    {
        auto nbgl = base_cast<NoclipBaseGameLayer*>(this);

        if (!nbgl->m_fields->hasDied)
        {
            nbgl->m_fields->hasDied = true;
            nbgl->m_fields->timeDead += CCDirector::get()->getDeltaTime();
        }

        SafeMode::get()->setHackedAttempt();
        
        if (m_player1)
            m_player1->m_maybeIsColliding = false;

        if (m_player2)
            m_player2->m_maybeIsColliding = false;
    }

    LevelEditorLayer::postUpdate(p0);
}