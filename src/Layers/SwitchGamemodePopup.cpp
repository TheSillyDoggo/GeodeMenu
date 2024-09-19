#include "SwitchGamemodePopup.hpp"

#define speedbtn(speedid) \
speed = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName(speedid), CCSprite::createWithSpriteFrameName(speedid), this, menu_selector(SwitchGamemodePopup::onSpeed)); \
as<CCSprite*>(speed->m_offButton->getNormalImage())->setColor(ccc3(175, 175, 175)); \
speedBtns.push_back(speed);

#define gamemodebtn(iconname) \
gamemode = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName(fmt::format("gj_{}Btn_off_001.png", iconname).c_str()), CCSprite::createWithSpriteFrameName(fmt::format("gj_{}Btn_on_001.png", iconname).c_str()), this, menu_selector(SwitchGamemodePopup::onGamemode)); \
gamemodeBtns.push_back(gamemode);

void SwitchGamemodePopup::customSetup()
{
    auto speeds = CCMenu::create();
    speeds->setLayout(AxisLayout::create(Axis::Row)->setGap(15)->setAutoScale(false));
    speeds->setScale(0.72f);

    auto gamemodes = CCMenu::create();
    gamemodes->setLayout(AxisLayout::create(Axis::Row)->setGap(15)->setAutoScale(false));
    gamemodes->setScale(0.8f);

    auto misc = CCMenu::create();
    misc->setLayout(AxisLayout::create(Axis::Row)->setGap(15)->setAutoScale(false));
    misc->setScale(0.8f);

    CCMenuItemToggler* speed;
    CCMenuItemToggler* gamemode;

    speedbtn("boost_01_001.png")
    speedbtn("boost_02_001.png")
    speedbtn("boost_03_001.png")
    speedbtn("boost_04_001.png")
    speedbtn("boost_05_001.png")

    gamemodebtn("icon")
    gamemodebtn("ship")
    gamemodebtn("ball")
    gamemodebtn("bird")
    gamemodebtn("dart")
    gamemodebtn("robot")
    gamemodebtn("spider")
    gamemodebtn("swing")

    mirror = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("portal_06_front_001.png"), CCSprite::createWithSpriteFrameName("portal_05_front_001.png"), this, menu_selector(SwitchGamemodePopup::onMirror));
    mirror->setScale(0.45f);
    //misc->addChild(mirror);

    for (size_t i = 0; i < speedBtns.size(); i++)
    {
        speedBtns[i]->setTag(i);
        speeds->addChild(speedBtns[i]);
    }

    for (size_t i = 0; i < gamemodeBtns.size(); i++)
    {
        gamemodeBtns[i]->setTag(i);
        gamemodes->addChild(gamemodeBtns[i]);
    }
    
    speeds->updateLayout();
    gamemodes->updateLayout();
    misc->updateLayout();
    baseLayer->addChildAtPosition(speeds, Anchor::Center, ccp(0, -15));
    baseLayer->addChildAtPosition(gamemodes, Anchor::Center, ccp(0, 40));
    baseLayer->addChildAtPosition(misc, Anchor::Center, ccp(0, -40));

    updateButtons();
}

void SwitchGamemodePopup::onSpeed(CCObject* sender)
{
    SafeMode::get()->setHackedAttempt();

    auto player = getPlayer();

    switch (sender->getTag())
    {
        case 0:
            player->m_playerSpeed = 0.7f;
            break;

        case 1:
            player->m_playerSpeed = 0.9f;
            break;

        case 2:
            player->m_playerSpeed = 1.1f;
            break;

        case 3:
            player->m_playerSpeed = 1.3f;
            break;

        case 4:
            player->m_playerSpeed = 1.6f;
            break;
    };

    updateButtons();
}

void SwitchGamemodePopup::onGamemode(CCObject* sender)
{
    SafeMode::get()->setHackedAttempt();

    auto player = getPlayer();

    switch (sender->getTag())
    {
        case 0:
            player->toggleFlyMode(false, false);
            player->toggleRollMode(false, false);
            player->toggleBirdMode(false, false);
            player->toggleDartMode(false, false);
            player->toggleRobotMode(false, false);
            player->toggleSpiderMode(false, false);
            player->toggleSwingMode(false, false);
            break;

        case 1:
            player->toggleFlyMode(true, true);
            break;

        case 2:
            player->toggleRollMode(true, true);
            break;

        case 3:
            player->toggleBirdMode(true, true);
            break;

        case 4:
            player->toggleDartMode(true, true);
            break;

        case 5:
            player->toggleRobotMode(true, true);
            break;

        case 6:
            player->toggleSpiderMode(true, true);
            break;

        case 7:
            player->toggleSwingMode(true, true);
            break;
    };

    auto obj = TeleportPortalObject::create("edit_eGameRotBtn_001.png", true);
    obj->m_cameraIsFreeMode = true;
    GJBaseGameLayer::get()->playerWillSwitchMode(player, obj);

    updateButtons();
}

void SwitchGamemodePopup::onMirror(CCObject* sender)
{
    GJBaseGameLayer::get()->toggleFlipped(!(GJBaseGameLayer::get()->m_objectLayer->getScaleX() == -1), true);

    updateButtons();
}

void SwitchGamemodePopup::updateButtons()
{
    auto player = getPlayer();

    for (size_t i = 0; i < speedBtns.size(); i++)
    {
        bool isCurr = false;

        if (i == 0) isCurr = player->m_playerSpeed == 0.7f;
        if (i == 1) isCurr = player->m_playerSpeed == 0.9f;
        if (i == 2) isCurr = player->m_playerSpeed == 1.1f;
        if (i == 3) isCurr = player->m_playerSpeed == 1.3f;
        if (i == 4) isCurr = player->m_playerSpeed == 1.6f;

        speedBtns[i]->setEnabled(!isCurr);
        speedBtns[i]->toggle(isCurr);
    }

    for (size_t i = 0; i < gamemodeBtns.size(); i++)
    {
        bool isCurr = getGamemodeIndex(player) == i;

        gamemodeBtns[i]->setEnabled(!isCurr);
        gamemodeBtns[i]->toggle(isCurr);
    }
}

PlayerObject* SwitchGamemodePopup::getPlayer()
{
    return GJBaseGameLayer::get()->m_player1;
}

int SwitchGamemodePopup::getGamemodeIndex(PlayerObject* player)
{
    if (player->m_isShip)
        return 1;

    if (player->m_isBall)
        return 2;

    if (player->m_isBird)
        return 3;
    
    if (player->m_isDart)
        return 4;
    
    if (player->m_isRobot)
        return 5;

    if (player->m_isSpider)
        return 6;

    if (player->m_isSwing)
        return 7;

    return 0;
}

SwitchGamemodePopup* SwitchGamemodePopup::create()
{
    auto pRet = new SwitchGamemodePopup();

    if (pRet && pRet->initWithSizeAndName(ccp(360, 200), "Switch Gamemode"))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

SwitchGamemodePopup* SwitchGamemodePopup::addToScene()
{
    auto pRet = SwitchGamemodePopup::create();

    CCScene::get()->addChild(pRet, 99999);

    return pRet;
}