#include "ZoomControl.hpp"
#include <EasyBG.hpp>
#include "ZoomInterceptNode.hpp"
#include "ZoomPause.hpp"

using namespace geode::prelude;
using namespace qolmod;

bool ZoomControl::init()
{
    if (!CCNode::init())
        return false;

    this->setPosition(ccp(
        Mod::get()->getSavedValue<float>("positions/zoom-control.x", 25),
        Mod::get()->getSavedValue<float>("positions/zoom-control.y", 25)
    ));
    this->setContentSize(ccp(28, 100));
    this->scheduleUpdate();

    auto bg = EasyBG::create();
    bg->setTargettingNode(this);
    bg->setAnchorPoint(ccp(0, 0));

    auto grabSpr = CCSprite::create("draggable.png"_spr);
    grabSpr->setPosition(ccp(getContentWidth() / 2, getContentWidth() / 2));
    grabSpr->setScale(0.6f);

    auto grab = GrabNodeLayer::create();
    grab->setContentSize(ccp(getContentWidth(), getContentWidth()));
    grab->setPosition(grabSpr->getPosition());
    grab->setNodeToGrab(this);
    grab->setLockedAxis(LockedAxis::Any);
    grab->setOnEndDrag([this]
    {
        Mod::get()->setSavedValue<float>("positions/zoom-control.x", this->getPositionX());
        Mod::get()->setSavedValue<float>("positions/zoom-control.y", this->getPositionY());
    });

    menu = CCMenu::create();
    menu->setPosition(ccp(
        getContentWidth() / 2,
        (getContentHeight() - getContentWidth()) / 2 + getContentWidth()
    ));
    menu->setAnchorPoint(ccp(0, 0));
    menu->setScale(0.5f);

    zoomIn = Button::create(CCSprite::createWithSpriteFrameName("GJ_zoomInBtn_001.png"), this, menu_selector(ZoomControl::onZoomIn));
    zoomOut = Button::create(CCSprite::createWithSpriteFrameName("GJ_zoomOutBtn_001.png"), this, menu_selector(ZoomControl::onZoomOut));
    label = CCLabelBMFont::create("100%", "bigFont.fnt");

    zoomIn->setPositionY(40);
    zoomOut->setPositionY(-50);
    label->setPositionY(-5);

    menu->addChild(label);
    menu->addChild(zoomIn);
    menu->addChild(zoomOut);

    this->addChild(bg);
    this->addChild(grabSpr);
    this->addChild(grab);
    this->addChild(menu);
    return true;
}

void ZoomControl::update(float dt)
{
    this->setVisible(ZoomPauseMenu::get()->getRealEnabled() && ShowZoomControls::get()->getRealEnabled());

    auto intercept = ZoomInterceptNode::get();

    if (!intercept)
    {
        this->removeFromParent();
        return;
    }

    label->setString(fmt::format("{}%", (int)((intercept->getZoom() + 1) * 100)).c_str());
    // label->limitLabelWidth(getContentWidth(), 1, 0);
}

void ZoomControl::onZoomIn(CCObject* sender)
{
    if (auto intercept = ZoomInterceptNode::get())
        intercept->setZoom(intercept->getZoom() + 0.5f);
}

void ZoomControl::onZoomOut(CCObject* sender)
{
    if (auto intercept = ZoomInterceptNode::get())
        intercept->setZoom(intercept->getZoom() - 0.5f);
}

ZoomControl::~ZoomControl()
{
    
}