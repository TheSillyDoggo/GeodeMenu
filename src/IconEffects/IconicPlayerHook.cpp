#include "IconicPlayerHook.hpp"
#include <ColourUtils.hpp>
#include <FineOutline.hpp>

IconicPlayerHook* IconicPlayerHook::create(PlayerObject* player)
{
    auto pRet = new IconicPlayerHook();
    pRet->player = player;
    player->setUserObject("iconic-hook"_spr, pRet);

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

    setEnabled(true);
    setUserObject("unpausable"_spr, CCNode::create());

    config = IconicManager::get()->getConfig(IconicGamemodeType::Cube, false);

    return true;
}

void IconicPlayerHook::update(float dt)
{
    if (IconicManager::get()->areIncompatibleModsLoaded())
        return;

    if (config)
    {
        if (simple)
        {
            simple->setColor(config->getPrimary());
            simple->setSecondColor(config->getSecondary());
            simple->setGlowOutline(config->getGlow());
            
            alpha::fine_outline::setOutlineColor(simple, config->getFineOutline());
        }

        if (player)
        {
            if (player->m_isShip)
            {
                config = IconicManager::get()->getConfig(player->m_isPlatformer ? IconicGamemodeType::Jetpack : IconicGamemodeType::Ship, player2);
                auto config2 = IconicManager::get()->getConfig(IconicGamemodeType::Cube, player2);

                player->setColor(config2->getPrimary());
                player->setSecondColor(config2->getSecondary());
                player->m_iconGlow->setColor(config2->getGlow());

                player->m_vehicleSprite->setColor(config->getPrimary());
                player->m_vehicleSpriteSecondary->setColor(config->getSecondary());
                player->m_vehicleGlow->setColor(config->getGlow());
            }
            else if (player->m_isBird)
            {
                config = IconicManager::get()->getConfig(IconicGamemodeType::Bird, player2);
                auto config2 = IconicManager::get()->getConfig(IconicGamemodeType::Cube, player2);

                player->setColor(config2->getPrimary());
                player->setSecondColor(config2->getSecondary());
                player->m_iconGlow->setColor(config2->getGlow());

                player->m_vehicleSprite->setColor(config->getPrimary());
                player->m_vehicleSpriteSecondary->setColor(config->getSecondary());
                player->m_vehicleGlow->setColor(config->getGlow());
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
                player->m_iconGlow->setColor(config->getGlow());

                if (player->m_robotSprite)
                {
                    for (auto child : player->m_robotSprite->m_glowSprite->getChildrenExt<CCSprite*>())
                        child->setColor(config->getGlow());
                }

                if (player->m_spiderSprite)
                {
                    for (auto child : player->m_spiderSprite->m_glowSprite->getChildrenExt<CCSprite*>())
                        child->setColor(config->getGlow());
                }
            }

            if (player->m_regularTrail)
                player->m_regularTrail->setColor(config->getTrail());

            if (player->m_ghostTrail)
                player->m_ghostTrail->m_color = config->getGhost();

            if (player->m_waveTrail)
            {
                player->m_waveTrail->setColor(config->getWaveTrail());

                if (!player->m_waveTrail->isRunning())
                    player->m_waveTrail->updateStroke(1);
            }

            if (IconicManager::get()->isFineOutlineLoaded())
            {
                alpha::fine_outline::setOutlineColor(player, config->getFineOutline());
            }

            for (auto fire : fireDashes)
            {
                if (fire.valid())
                    fire.lock()->setColor(config->getDashFire());
            }

            for (auto spider : spiderTeleports)
            {
                if (spider.valid())
                    spider.lock()->setColor(config->getSpiderTeleport());
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

void IconicPlayerHook::onBeginDash(CCSprite* sprite)
{
    fireDashes.push_back(sprite);
}

void IconicPlayerHook::onSpiderTeleport(CCSprite* sprite)
{
    spiderTeleports.push_back(sprite);
}