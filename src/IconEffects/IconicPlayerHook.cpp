#include "IconicPlayerHook.hpp"
#include <ColourUtils.hpp>

IconicPlayerHook* IconicPlayerHook::create(PlayerObject* player)
{
    auto pRet = new IconicPlayerHook();
    pRet->player = player;

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

IconicPlayerHook* IconicPlayerHook::create(SimplePlayer* simple)
{
    auto pRet = new IconicPlayerHook();
    pRet->simple = simple;

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool IconicPlayerHook::init()
{
    if (!CCNode::init())
        return false;

    saveDefault();
    setEnabled(true);

    config = IconicManager::get()->getConfig(IconicGamemodeType::Cube, false);

    return true;
}

void IconicPlayerHook::update(float dt)
{
    if (config)
    {
        if (simple)
        {
            simple->setColor(config->getPrimary());
            simple->setSecondColor(config->getSecondary());
            simple->setGlowOutline(config->getGlow());
        }
    }
}

void IconicPlayerHook::setEnabled(bool enabled)
{
    if (this->enabled == enabled)
        return;

    this->enabled = enabled;

    if (enabled)
    {
        scheduleUpdate();
        update(0);
    }
    else
    {
        unscheduleUpdate();
        restoreDefault();
    }
}

bool IconicPlayerHook::isEnabled()
{
    return enabled;
}

void IconicPlayerHook::setGamemode(IconicGamemodeType gamemode, bool player2)
{
    config = IconicManager::get()->getConfig(gamemode, player2);
}

void IconicPlayerHook::saveDefault()
{
    if (simple)
    {
        values.primary = simple->m_firstLayer->getColor();
        values.secondary = simple->m_secondLayer->getColor();
        values.glow = simple->m_glowColor;
    }

    if (player)
    {
        auto gh = player->m_ghostType;
        player->toggleGhostEffect(GhostType::Enabled);

        values.ghost = player->m_ghostTrail->m_color;
        // values.trail = player->m_ghostTrail
        // values.wave = player->m_ghostTrail

        player->toggleGhostEffect(gh);
    }
}

void IconicPlayerHook::restoreDefault()
{
    if (simple)
    {
        simple->setColor(values.primary);
        simple->setSecondColor(values.secondary);
        simple->setGlowOutline(values.glow);
    }
}