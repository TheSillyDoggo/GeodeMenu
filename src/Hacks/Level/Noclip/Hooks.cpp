#include "Hooks.hpp"
#include "../../../Labels/Nodes/LabelContainerLayer.hpp"

#define NOCLIP_BASE() base_cast<NoclipBaseGameLayer*>(this)

float NoclipBaseGameLayer::getNoclipAccuracy(NoclipPlayerSelector selector)
{
    // m_gameState.m_levelTime is time, m_currentProgress is ticks
    if (m_fields->timeInLevel == 0)
        return 1;

    int ticks = 0;

    switch (selector)
    {
        case NoclipPlayerSelector::All:
            ticks = m_fields->totalTicksDead;
            break;

        case NoclipPlayerSelector::Player1:
            ticks = m_fields->p1TicksDead;
            break;

        case NoclipPlayerSelector::Player2:
            ticks = m_fields->p2TicksDead;
            break;
    }

    return 1 - ((float)ticks / (float)m_fields->timeInLevel);
}

int NoclipBaseGameLayer::getNoclipDeaths(NoclipPlayerSelector selector)
{
    return 0;
}

void NoclipBaseGameLayer::resetNoclipValues()
{
    auto fields = m_fields.self();

    fields->totalTicksDead = 0;
    fields->p1TicksDead = 0;
    fields->p2TicksDead = 0;

    fields->totalDeaths = 0;
    fields->p1Deaths = 0;
    fields->p2Deaths = 0;
}

void NoclipBaseGameLayer::playerDied(NoclipPlayerSelector selector)
{
    auto fields = m_fields.self();

    if (!fields->hasDiedThisTick)
    {
        fields->hasDiedThisTick = true;

        fields->totalTicksDead++;

        if (selector == NoclipPlayerSelector::Player1)
        {
            fields->p1TicksDead++;
            fields->hasP1DiedThisTick = true;
        }
        else
        {
            fields->p2TicksDead++;
            fields->hasP2DiedThisTick = true;
        }
    }

    if (LabelContainerLayer::get())
        LabelContainerLayer::get()->onEventTriggered(LabelEventType::PlayerTookDamage);
}

void NoclipBaseGameLayer::onPostUpdate()
{
    auto fields = m_fields.self();

    if (!fields->didDieLastTick && fields->hasDiedThisTick)
        fields->totalDeaths++;

    if (!fields->didP1DieLastTick && fields->hasP1DiedThisTick)
        fields->p1Deaths++;

    if (!fields->didP2DieLastTick && fields->hasP2DiedThisTick)
        fields->p2Deaths++;

    fields->didDieLastTick = fields->hasDiedThisTick;
    fields->hasDiedThisTick = false;

    fields->didP1DieLastTick = fields->hasP1DiedThisTick;
    fields->hasP1DiedThisTick = false;

    fields->didP2DieLastTick = fields->hasP2DiedThisTick;
    fields->hasP2DiedThisTick = false;

    if (auto pl = typeinfo_cast<PlayLayer*>(this))
    {
        if (!pl->m_levelEndAnimationStarted)
            fields->timeInLevel = m_gameState.m_currentProgress;
    }
    else
    {
        fields->timeInLevel = m_gameState.m_currentProgress;
    }
}

class $modify (PlayLayer)
{
    struct Fields
    {
        CCLayerColor* tintOverlay;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        m_fields->tintOverlay = CCLayerColor::create(ccc4(0, 0, 0, 0));
        m_fields->tintOverlay->setID("noclip-tint-overlay"_spr);
        m_fields->tintOverlay->setZOrder(1);
        
        this->addChild(m_fields->tintOverlay);
        return true;
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        if (p1 == m_anticheatSpike)
            return PlayLayer::destroyPlayer(p0, p1);

        if (!Noclip::get()->getRealEnabled())
            return PlayLayer::destroyPlayer(p0, p1);

        NOCLIP_BASE()->playerDied(p0 == m_player1 ? NoclipPlayerSelector::Player1 : NoclipPlayerSelector::Player2);

        if (NoclipTintOnDeath::get()->getRealEnabled())
        {
            m_fields->tintOverlay->stopAllActions();
            m_fields->tintOverlay->setOpacity(NoclipTintOpacity::get()->getValue() * 255.0f);
            m_fields->tintOverlay->runAction(CCFadeTo::create(0.35f, 0));
        }
    }

    void resetLevel()
    {
        PlayLayer::resetLevel();

        NOCLIP_BASE()->resetNoclipValues();
    }

    virtual void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);
        NOCLIP_BASE()->onPostUpdate();

        m_fields->tintOverlay->setColor(NoclipTintColour::get()->getColour());
    }
};