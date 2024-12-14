#include "SillyBaseLayer.h"
#include "../Utils/LaunchArgs.hpp"
#include "../Utils/UnspeedhackedAction.hpp"

bool SillyBaseLayer::setup()
{
    m_mainLayer->setVisible(false);
    this->stopAllActions();
    this->setOpacity(0);

    if (!noBlur && Client::GetModuleEnabled("menu-bg-blur") && !LaunchArgs::get()->hasLaunchArg("--qolmod:no-blur"))
    {
        blur = BlurLayer::create();
        blur->runAction(UnspeedhackedAction::create(CCEaseIn::create(CCFadeTo::create(0.5f, 255), 2)));
        this->addChild(blur);
    }

    this->runAction(CCFadeTo::create(1, 100));
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    auto l = CCMenu::create();
    l->setContentSize(size);
    l->setPosition(CCDirector::get()->getWinSize() / 2);
    l->ignoreAnchorPointForPosition(false);

    bg = QOLModMenuBG::create();
    bg->setContentSize(size);

    l->addChildAtPosition(bg, Anchor::Center);

    title = TransLabelBMFont::create(_title.c_str(), "bigFont.fnt");
    title->setPosition(l->getContentSize() / 2 + ccp(0, (l->getContentSize().height / 2) - 15));
    title->setScale(0.6f);
    l->addChild(title);

    if (createWithOK)
    {
        ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SillyBaseLayer::onClose));
        ok->setPosition(l->getContentSize() / 2 + ccp(0, -l->getContentSize().height / 2 + 23));
        l->addChild(ok);
    }

    if (animate)
    {
        auto action = UnspeedhackedAction::create(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1), 0.6f));
        action->setTag(69);

        l->setScale(0);
        l->runAction(action);
    }

    this->addChild(l);

    baseLayer = l;

    this->customSetup();

    return true;
}

bool SillyBaseLayer::initWithSizeAndName(CCPoint size, std::string _title, bool createWithOK, bool animate, bool noBlur)
{
    this->_title = _title;
    this->createWithOK = createWithOK;
    this->animate = animate;
    this->noBlur = noBlur;
    this->size = size;

    if (!initAnchored(69, 420))
        return false;

    return true;
}

bool SillyBaseLayer::ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
{
    return true;
}

void SillyBaseLayer::ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
{

}

void SillyBaseLayer::ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
{

}

void SillyBaseLayer::ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
{

}

SillyBaseLayer::~SillyBaseLayer()
{

}