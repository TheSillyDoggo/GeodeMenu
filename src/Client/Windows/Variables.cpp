#include "Variables.h"

using namespace geode::prelude;

void Variables::onChangeMode(CCObject* sender) {
    log::info("button");

    //PlayLayer::get()->m_player1->toggleDartMode(true, true);
    //PlayLayer::get()->m_player1->m_position += ccp(30, 150);

    //PlayLayer::get()->m_player1->playerDestroyed(false);

    //PlayLayer::get()->destroyPlayer(PlayLayer::get()->m_player1, GameObject::createWithKey(0));
    /*
    //why robert

    bool m_isShip;
    bool m_isBall;
    bool m_isBird;
    bool m_isDart;
    bool m_isRobot;
    bool m_isSpider;
    bool m_isSwing;
    */

    //if (!PlayLayer::get() || !PlayLayer::get()->m_player1)
        //return;

    //auto plr = PlayLayer::get()->m_player1;

    //int selMode = 5;

    //plr->m_isPlatformer = true;

    //plr->switchedToMode(GameObjectType::BallPortal);

    //plr->toggleRollMode(true, true);
    //plr->m_isBall = selMode == 2;
    //plr->m_isBird = selMode == 3;
    //plr->m_isDart = selMode == 4;
    //plr->m_isRobot = selMode == 5;
    //plr->m_isSpider = selMode == 6;
    //plr->m_isSwing = selMode == 7;

    //plr->updatePlayerFrame();
    //plr->updatePlayerShipFrame();
}

void cocosCreate(CCMenu* menu) {
    auto back = CCScale9Sprite::create("square02_small.png");
    back->setContentSize(ccp(110, menu->getContentSize().height) / 0.5f);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setOpacity(100);

    auto btn = CCMenuItemSpriteExtra::create(back, menu, menu_selector(Variables::onChangeMode));
    menu->addChild(btn);
}