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

        if (player)
        {
            if (player->m_isShip)
            {
                config = IconicManager::get()->getConfig(IconicGamemodeType::Cube, player2);
                auto config2 = IconicManager::get()->getConfig(player->m_isPlatformer ? IconicGamemodeType::Jetpack : IconicGamemodeType::Ship, player2);

                player->setColor(config->getPrimary());
                player->setSecondColor(config->getSecondary());
                player->setGlowColor(config->getGlow());

                player->m_vehicleSprite->setColor(config2->getPrimary());
                player->m_vehicleSpriteSecondary->setColor(config2->getSecondary());
                player->m_vehicleGlow->setColor(config2->getGlow());
            }
            else if (player->m_isBird)
            {
                config = IconicManager::get()->getConfig(IconicGamemodeType::Cube, player2);
                auto config2 = IconicManager::get()->getConfig(IconicGamemodeType::Bird, player2);

                player->setColor(config->getPrimary());
                player->setSecondColor(config->getSecondary());
                player->setGlowColor(config->getGlow());

                player->m_vehicleSprite->setColor(config2->getPrimary());
                player->m_vehicleSpriteSecondary->setColor(config2->getSecondary());
                player->m_vehicleGlow->setColor(config2->getGlow());
            }
            else
            {
                if (player->m_isBall)
                    config = IconicManager::get()->getConfig(IconicGamemodeType::Ball, player2);
                else if (player->m_isDart)
                    config = IconicManager::get()->getConfig(IconicGamemodeType::Dart, player2);
                else if (player->m_isRobot)
                    config = IconicManager::get()->getConfig(IconicGamemodeType::Robot, player2);
                else if (player->m_isSpider)
                    config = IconicManager::get()->getConfig(IconicGamemodeType::Spider, player2);
                else if (player->m_isSwing)
                    config = IconicManager::get()->getConfig(IconicGamemodeType::Swing, player2);
                else
                    config = IconicManager::get()->getConfig(IconicGamemodeType::Cube, player2);

                player->setColor(config->getPrimary());
                player->setSecondColor(config->getSecondary());
                player->setGlowColor(config->getGlow());
            }
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
    this->player2 = player2;
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
        // auto gh = player->m_ghostType;
        // player->toggleGhostEffect(GhostType::Enabled);

        // values.ghost = player->m_ghostTrail->m_color;
        // values.trail = player->m_ghostTrail
        // values.wave = player->m_ghostTrail

        // player->toggleGhostEffect(gh);
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