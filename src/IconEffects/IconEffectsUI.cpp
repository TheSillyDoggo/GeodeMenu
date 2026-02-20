#include "IconEffectsUI.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../GUI/BetterInputNode.hpp"
#include <SetupColourConfigUI.hpp>
#include "IconicManager.hpp"
#include <BetterButtonSprite.hpp>
#include <ColourUtils.hpp>
#include <Nodes/CCNodeWithShader.hpp>

bool IconEffectsUI::init()
{
    if (!CategoryNode::init())
        return false;

    this->scheduleUpdate();

    auto n = CCNode::create();
    n->addChild(getPreviewNode(IconicGamemodeType::Cube));
    n->addChild(getPreviewNode(IconicGamemodeType::Ship));
    n->addChild(getPreviewNode(IconicGamemodeType::Jetpack));
    n->addChild(getPreviewNode(IconicGamemodeType::Ball));

    auto menu = CCMenu::create();
    menu->setContentSize(ccp(100, 100));

    auto btn = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Primary", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest));
    auto btn2 = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Secondary", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest2));
    auto btn3 = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Glow", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest3));

    menu->addChildAtPosition(btn, Anchor::TopRight);
    menu->addChildAtPosition(btn2, Anchor::Right);
    menu->addChildAtPosition(btn3, Anchor::BottomRight);
    this->addChild(menu);
    this->addChild(n);
    return true;
}

CCNode* IconEffectsUI::getPreviewNode(IconicGamemodeType gamemode)
{
    int id = 1;
    IconType type;
    auto gm = GameManager::get();

    switch (gamemode)
    {
        case IconicGamemodeType::Cube:
            id = gm->m_playerFrame;
            type = IconType::Cube;
            break;
        case IconicGamemodeType::Ship:
            id = gm->m_playerShip;
            type = IconType::Ship;
            break;
        case IconicGamemodeType::Jetpack:
            id = gm->m_playerJetpack;
            type = IconType::Jetpack;
            break;
        case IconicGamemodeType::Ball:
            id = gm->m_playerBall;
            type = IconType::Ball;
            break;
        case IconicGamemodeType::Ufo:
            id = gm->m_playerBird;
            type = IconType::Ufo;
            break;
        case IconicGamemodeType::Wave:
            id = gm->m_playerDart;
            type = IconType::Wave;
            break;
        case IconicGamemodeType::Robot:
            id = gm->m_playerRobot;
            type = IconType::Robot;
            break;
        case IconicGamemodeType::Spider:
            id = gm->m_playerSpider;
            type = IconType::Spider;
            break;
        case IconicGamemodeType::Swing:
            id = gm->m_playerSwing;
            type = IconType::Swing;
            break;
    }

    auto plr = SimplePlayer::create(id);
    plr->updatePlayerFrame(id, type);

    auto hk = IconicPlayerHook::create(plr);
    hk->setGamemode(gamemode, false);
    plr->addChild(hk);

    auto n = CCNodeWithShader::create();
    n->addChild(plr);

    return n;
}

void IconEffectsUI::onTest(CCObject* sender)
{
    auto conf = IconicManager::get()->getConfig(IconicGamemodeType::Cube, false);

    auto ui = SetupColourConfigUI::create([this, conf](ColourConfig con)
    {
        conf->setPrimaryConfig(con);
    });
    ui->setStartConfig(conf->getPrimaryConfig());
    ui->show();
}

void IconEffectsUI::onTest2(CCObject* sender)
{
    auto conf = IconicManager::get()->getConfig(IconicGamemodeType::Cube, false);

    auto ui = SetupColourConfigUI::create([this, conf](ColourConfig con)
    {
        conf->setSecondaryConfig(con);
    });
    ui->setStartConfig(conf->getSecondaryConfig());
    ui->show();
}

void IconEffectsUI::onTest3(CCObject* sender)
{
    auto conf = IconicManager::get()->getConfig(IconicGamemodeType::Cube, false);

    auto ui = SetupColourConfigUI::create([this, conf](ColourConfig con)
    {
        conf->setGlowConfig(con);
    });
    ui->setStartConfig(conf->getGlowConfig());
    ui->show();
}

void IconEffectsUI::update(float dt)
{
    
}