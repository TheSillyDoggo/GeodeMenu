#include "Noclip.hpp"
#include "../SafeMode/SafeMode.hpp"
#include "../../Labels/LabelLayer.hpp"

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

    if (!Client::GetModuleEnabled("noclip-player2") && p0 == m_player2)
        return PlayLayer::destroyPlayer(p0, p1);

    if (!Client::GetModuleEnabled("noclip-player1") && p0 == m_player1)
        return PlayLayer::destroyPlayer(p0, p1);

    if (!Client::GetModuleEnabled("noclip") || (base_cast<NoclipBaseGameLayer*>(this)->m_fields->ac == p1))
        PlayLayer::destroyPlayer(p0, p1);
    else
    {
        if (Client::GetModuleEnabled("noclip-min-accuracy-toggle"))
        {
            static InputModule* noclipLimit = nullptr;
            if (!noclipLimit)
                noclipLimit = as<InputModule*>(Client::GetModule("noclip")->options[5]);

            if (base_cast<NoclipBaseGameLayer*>(this)->getNoclipAccuracy() * 100 < noclipLimit->getFloatValue())
                return PlayLayer::destroyPlayer(p0, p1);
        }

        SafeMode::get()->setHackedAttempt("Player Died with Noclip Enabled");

        if (LabelLayer::get())
            LabelLayer::get()->triggerEvent(LabelEventType::PlayerTookDamage);

        if (!base_cast<NoclipBaseGameLayer*>(this)->m_fields->hasDied)
        {
            base_cast<NoclipBaseGameLayer*>(this)->m_fields->hasDied = true;
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

            m_fields->tint->setColor(as<ColourModule*>(Client::GetModule("noclip")->options[2])->colour);
        }

        if (p0 == m_player1)
            base_cast<NoclipBaseGameLayer*>(this)->playSFX(true);

        if (p0 == m_player2)
            base_cast<NoclipBaseGameLayer*>(this)->playSFX(false);
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

void NoclipBaseGameLayer::checkRepellPlayer()
{
    GJBaseGameLayer::checkRepellPlayer();

    m_fields->isTickUpdate = true;
}

void NoclipBaseGameLayer::updateCamera(float dt)
{
    GJBaseGameLayer::updateCamera(dt);

    if (!shouldIncreaseTime())
        return;

    if (m_fields->isTickUpdate == true)
    {
        m_fields->isTickUpdate = false;

        if (m_fields->hasDied)
        {
            m_fields->timeDead += dt / 60.0f;
            m_fields->hasDied = false;
        }

        m_fields->timeInLevel += dt / 60.0f;
    }
}

void NoclipBaseGameLayer::resetLevelVariables()
{
    GJBaseGameLayer::resetLevelVariables();

    m_fields->timeDead = 0;
    m_fields->timeInLevel = 0;
}

float NoclipBaseGameLayer::getNoclipAccuracy()
{
    if (m_fields->timeInLevel == 0)
        return 1;

    return 1 - (m_fields->timeDead / m_fields->timeInLevel);
}

int NoclipBaseGameLayer::getNoclipDeaths()
{
    if (auto pl = typeinfo_cast<PlayLayer*>(this))
        return as<NoclipPlayLayer*>(pl)->m_fields->d;
    else
        return 0;
}

bool NoclipBaseGameLayer::shouldIncreaseTime()
{
    return !m_levelEndAnimationStarted;
}

void NoclipEditorLayer::playerTookDamage(PlayerObject* p0)
{
    if (!Client::GetModuleEnabled("noclip-player2") && p0 == m_player2)
        return LevelEditorLayer::playerTookDamage(p0);

    if (!Client::GetModuleEnabled("noclip-player1") && p0 == m_player1)
        return LevelEditorLayer::playerTookDamage(p0);

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

        if (LabelLayer::get())
            LabelLayer::get()->triggerEvent(LabelEventType::PlayerTookDamage);

        SafeMode::get()->setHackedAttempt("Player Died with Noclip Enabled");
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

        SafeMode::get()->setHackedAttempt("Player Died with Noclip Enabled");
        
        if (m_player1)
            m_player1->m_maybeIsColliding = false;

        if (m_player2)
            m_player2->m_maybeIsColliding = false;
    }

    LevelEditorLayer::postUpdate(p0);
}

void NoclipBaseGameLayer::playSFX(bool player1)
{
    static SFXModule* p1sfx = nullptr;
    static SFXModule* p2sfx = nullptr;

    if (!p1sfx)
    {
        p1sfx = as<SFXModule*>(Client::GetModule("noclip-player1-sfx"));
        p2sfx = as<SFXModule*>(Client::GetModule("noclip-player2-sfx"));
    }

    if (player1)
    {
        if (m_fields->canP1SFXPlay)
        {
            m_fields->canP1SFXPlay = false;

            p1sfx->playRandomSFX();

            this->scheduleOnce(schedule_selector(NoclipBaseGameLayer::resetP1SFX), 0.3f);
        }
    }
    else
    {
        if (m_fields->canP2SFXPlay)
        {
            m_fields->canP2SFXPlay = false;

            p2sfx->playRandomSFX();

            this->scheduleOnce(schedule_selector(NoclipBaseGameLayer::resetP2SFX), 0.3f);
        }
    }
}

void NoclipBaseGameLayer::resetP1SFX(float)
{
    m_fields->canP1SFXPlay = true;
}

void NoclipBaseGameLayer::resetP2SFX(float)
{
    m_fields->canP2SFXPlay = true;
}