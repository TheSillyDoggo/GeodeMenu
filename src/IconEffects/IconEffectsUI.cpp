#include "IconEffectsUI.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../GUI/BetterInputNode.hpp"
#include <SetupColourConfigUI.hpp>
#include "IconicManager.hpp"
#include <BetterButtonSprite.hpp>
#include <ColourUtils.hpp>

bool IconEffectsUI::init()
{
    if (!CategoryNode::init())
        return false;

    this->scheduleUpdate();

    bg->setAnchorPoint(ccp(0.5f, 1));
    static_cast<AnchorLayoutOptions*>(bg->getLayoutOptions())->setAnchor(Anchor::Top);
    bg->setContentHeight(70);
    updateLayout();

    int x = 0;
    auto n = CCMenu::create();

    for (size_t i = 1; i < (int)IconicGamemodeType::Swing + 1; i++)
    {
        auto p1 = createPreviewButton((IconicGamemodeType)i, false);
        auto p2 = createPreviewButton((IconicGamemodeType)i, true);

        p1->setPosition(ccp(x, 20));
        p2->setPosition(ccp(x, -20));

        x += p1->getContentWidth() + 15;

        n->addChild(p1);
        n->addChild(p2);
    }
    
    n->setContentSize(ccp(x - 25, 0));
    n->setPosition(ccp(2.5f + 30 / 2, getContentHeight() - 35));
    n->setAnchorPoint(ccp(0, 0));
    n->setScale(0.85f);

    updateSelection();

    auto menu = CCMenu::create();
    menu->setContentSize(ccp(100, 100));
    menu->setPosition(ccp(100, 100));

    auto btn = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Primary", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest));
    auto btn2 = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Secondary", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest2));
    auto btn3 = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Glow", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest3));


    auto menu2 = CCMenu::create();
    menu2->setContentSize(ccp(100, 100));
    menu2->setPosition(ccp(30, 100));

    auto b2tn = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Invert", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest4));
    auto b2tn2 = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Same", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest4));
    auto b2tn3 = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(CCSizeMake(100, 30), "Seperate", "bigFont.fnt", "GJ_button_05.png"), this, menu_selector(IconEffectsUI::onTest4));
    b2tn->setTag(1);
    b2tn2->setTag(2);
    b2tn3->setTag(3);

    auto toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(IconEffectsUI::onTest5), 1.0f);
    toggler->setPosition(ccp(-50, -50));
    toggler->toggle(IconicManager::get()->getSeperateColours());

    menu->addChildAtPosition(btn, Anchor::TopRight);
    menu->addChildAtPosition(btn2, Anchor::Right);
    menu->addChildAtPosition(btn3, Anchor::BottomRight);
    menu2->addChildAtPosition(b2tn, Anchor::TopRight);
    menu2->addChildAtPosition(b2tn2, Anchor::Right);
    menu2->addChildAtPosition(b2tn3, Anchor::BottomRight);
    menu2->addChild(toggler);
    this->addChild(menu);
    this->addChild(menu2);
    this->addChild(n);
    return true;
}

CCMenuItemSpriteExtra* IconEffectsUI::createPreviewButton(IconicGamemodeType gamemode, bool player2)
{
    PreviewNodeData data;
    data.gamemode = gamemode;
    data.player2 = player2;

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
    plr->setScale(0.9f);

    auto hk = IconicPlayerHook::create(plr);
    hk->setGamemode(gamemode, player2);
    plr->addChild(hk);

    auto n = CCNodeWithShader::create();
    n->addChild(plr);

    auto btn = CCMenuItemSpriteExtra::create(n, this, menu_selector(IconEffectsUI::onSelectType));
    btn->setTag((int)gamemode);
    btn->setUserFlag("player2", player2);
    btn->setContentSize(ccp(30, 30));

    if (gamemode == IconicGamemodeType::Dart)
        btn->setContentSize(ccp(27.5f, 27.5f));

    n->setPosition(btn->getContentSize() / 2);

    data.hook = hk;
    data.shader = n;
    data.simple = plr;
    data.btn = btn;
    previewNodes.push_back(data);
    return btn;
}

std::pair<IconicGamemodeType, bool> IconEffectsUI::getSelected()
{
    IconicGamemodeType gamemode = selectedType.first;
    bool player2 = selectedType.second;

    if (IconicManager::get()->getDualMode() != IconicDualMode::Seperate)
        player2 = false;

    if (!IconicManager::get()->getSeperateColours())
        gamemode = IconicGamemodeType::Cube;

    return { gamemode, player2 };
}

void IconEffectsUI::updateSelection()
{
    for (auto data : previewNodes)
    {
        bool chk1 = selectedType.first == data.gamemode;
        bool chk2 = selectedType.second == data.player2;

        if (IconicManager::get()->getDualMode() != IconicDualMode::Seperate)
            chk2 = true;

        if (!IconicManager::get()->getSeperateColours())
            chk1 = true;

        bool sel = chk1 && chk2;

        data.btn->setEnabled(!sel);
        data.shader->setColor(sel ? ccc3(255, 255, 255) : ccc3(100, 100, 100));
    }
}

void IconEffectsUI::onSelectType(CCObject* sender)
{
    auto n = static_cast<CCNode*>(sender);
    selectedType.first = (IconicGamemodeType)n->getTag();
    selectedType.second = n->getUserFlag("player2");

    auto circle = CCCircleWave::create(0, 25, 0.2f, true);
    circle->setPosition(n->getContentSize() / 2);
    circle->m_circleMode = CircleMode::Outline;

    n->addChild(circle);

    updateSelection();
}

void IconEffectsUI::onTest(CCObject* sender)
{
    auto conf = IconicManager::get()->getConfig(getSelected().first, getSelected().second);

    auto ui = SetupColourConfigUI::create([this, conf](ColourConfig con)
    {
        conf->setPrimaryConfig(con);
    });
    ui->setStartConfig(conf->getPrimaryConfig());
    ui->show();
}

void IconEffectsUI::onTest2(CCObject* sender)
{
    auto conf = IconicManager::get()->getConfig(getSelected().first, getSelected().second);

    auto ui = SetupColourConfigUI::create([this, conf](ColourConfig con)
    {
        conf->setSecondaryConfig(con);
    });
    ui->setStartConfig(conf->getSecondaryConfig());
    ui->show();
}

void IconEffectsUI::onTest3(CCObject* sender)
{
    auto conf = IconicManager::get()->getConfig(getSelected().first, getSelected().second);

    auto ui = SetupColourConfigUI::create([this, conf](ColourConfig con)
    {
        conf->setGlowConfig(con);
    });
    ui->setStartConfig(conf->getGlowConfig());
    ui->show();
}

void IconEffectsUI::onTest4(CCObject* sender)
{
    switch (sender->getTag())
    {
        case 1:
            IconicManager::get()->setDualMode(IconicDualMode::Invert);
            break;

        case 2:
            IconicManager::get()->setDualMode(IconicDualMode::Same);
            break;

        case 3:
            IconicManager::get()->setDualMode(IconicDualMode::Seperate);
            break;
    }

    updateSelection();
}

void IconEffectsUI::onTest5(CCObject* sender)
{
    IconicManager::get()->setSeperateColours(!IconicManager::get()->getSeperateColours());

    updateSelection();
}

void IconEffectsUI::update(float dt)
{
    
}