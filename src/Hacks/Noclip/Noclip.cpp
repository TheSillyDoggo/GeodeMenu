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

    if (!m_fields->ac)
        m_fields->ac = p1;

    if (!Client::GetModuleEnabled("noclip") || (m_fields->ac == p1))
        PlayLayer::destroyPlayer(p0, p1);
    else
    {
        SafeMode::get()->setHackedAttempt();

        if (!m_fields->hasDied)
        {
            m_fields->hasDied = true;
            m_fields->timeDead += CCDirector::get()->getDeltaTime();
        }
        
        m_fields->isDead = true;

        if (m_fields->last != p1)
        {
            m_fields->last = p1;
            
            m_fields->d++;
        }

        if (m_fields->ac != p1)
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
    m_fields->timeDead = 0;
    m_fields->isDead = false;
}

float NoclipPlayLayer::getNoclipAccuracy()
{
    if (m_gameState.m_currentProgress == 0)
        return 1;

    return 1 - (m_fields->timeDead / as<float>(m_gameState.m_levelTime));
}

void NoclipBaseGameLayer::update(float dt)
{
    GJBaseGameLayer::update(dt);

    if (auto layer = typeinfo_cast<PlayLayer*>(this))
    {
        as<NoclipPlayLayer*>(layer)->m_fields->hasDied = false;
    }
}