#include "Universal.hpp"
#include "Level.hpp"

using namespace geode::prelude;

void Universal::cocosCreate(CCMenu* menu) {
    Window::cocosCreate(menu);

    auto pos = ccp(menu->getContentSize().width, 0) + ccp(-58, 22);

    CCMenuItemSpriteExtra* btn = nullptr;

    auto btnUnc = ButtonSprite::create("Uncomplete\nLevel", 70, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
    
    as<CCNode*>(btnUnc->getChildren()->objectAtIndex(0))->setScale(0.375f);
    as<CCLabelBMFont*>(btnUnc->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    as<CCLabelBMFont*>(btnUnc->getChildren()->objectAtIndex(0))->updateLabel();

    btn = CCMenuItemSpriteExtra::create(btnUnc, menu, menu_selector(Level::onUnc));
    btn->setSizeMult(1.15f);
    btn->setPosition(pos);
    menu->addChild(btn);
}