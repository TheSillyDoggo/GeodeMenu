#include "SmartStartposUI.hpp"

SmartStartposUIModule::SmartStartposUIModule()
{
    addToCache();
}

void SmartStartposUIModule::makeAndroid(CCNode* menuu, CCPoint pos)
{
    auto menu = CCMenu::create();
    menu->setPosition(sizeForOptionsPage() / 2);
    menu->setScale(0.6f);
    menu->setLayout(AxisLayout::create(Axis::Row)->setGap(40));

    auto speedUnsel = CCSprite::createWithSpriteFrameName("boost_03_001.png");
    speedUnsel->setColor(ccc3(125, 125, 125));
    auto speedSel = CCSprite::createWithSpriteFrameName("boost_03_001.png");
    auto speedBtn = CCMenuItemToggler::create(speedUnsel, speedSel, this, nullptr);

    auto sizeUnsel = CCSprite::createWithSpriteFrameName("portal_09_front_001.png");
    sizeUnsel->setColor(ccc3(125, 125, 125));
    auto sizeSel = CCSprite::createWithSpriteFrameName("portal_09_front_001.png");
    auto sizeBtn = CCMenuItemToggler::create(sizeUnsel, sizeSel, this, nullptr);

    auto modeUnsel = CCSprite::createWithSpriteFrameName("portal_13_front_001.png");
    modeUnsel->setColor(ccc3(125, 125, 125));
    auto modeSel = CCSprite::createWithSpriteFrameName("portal_13_front_001.png");
    auto modeBtn = CCMenuItemToggler::create(modeUnsel, modeSel, this, nullptr);

    auto dualUnsel = CCSprite::createWithSpriteFrameName("portal_11_front_001.png");
    dualUnsel->setColor(ccc3(125, 125, 125));
    auto dualSel = CCSprite::createWithSpriteFrameName("portal_11_front_001.png");
    auto dualBtn = CCMenuItemToggler::create(dualUnsel, dualSel, this, nullptr);

    auto gravUnsel = CCSprite::createWithSpriteFrameName("portal_02_front_001.png");
    gravUnsel->setColor(ccc3(125, 125, 125));
    auto gravSel = CCSprite::createWithSpriteFrameName("portal_02_front_001.png");
    auto gravBtn = CCMenuItemToggler::create(gravUnsel, gravSel, this, nullptr);

    menu->addChild(speedBtn);
    menu->addChild(sizeBtn);
    menu->addChild(modeBtn);
    menu->addChild(dualBtn);
    menu->addChild(gravBtn);
    menu->updateLayout();

    menuu->addChild(menu);
}

CCSize SmartStartposUIModule::sizeForOptionsPage()
{
    return ccp(350, 170);
}

void SmartStartposUIModule::save() { }

void SmartStartposUIModule::load() { }