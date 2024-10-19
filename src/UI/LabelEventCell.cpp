#include "LabelEventCell.hpp"
#include "../Layers/EditLabelPopup.hpp"

bool LabelEventCell::initWithEvent(LabelEvent* event)
{
    if (!CCNode::init())
        return false;

    this->setContentSize(ccp(243.25f, 50));
    this->setAnchorPoint(ccp(0, 1));

    this->event = event;

    auto bg = CCScale9Sprite::create("square02b_001.png");
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);
    bg->setID("background");
    bg->setScale(1.0f / 2.5f);
    bg->setContentSize(getContentSize() + ccp(-5, -5));
    bg->setContentSize(bg->getContentSize() * 2.5f);
    bg->setZOrder(-42069);
    
    auto menu = CCMenu::create();
    menu->setPosition(CCPointZero);

    auto delSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    delSpr->setScale(0.7f);

    auto del = CCMenuItemSpriteExtra::create(delSpr, this, menu_selector(LabelEventCell::onDelete));
    del->setPosition(this->getContentSize() * ccp(1, 0.5f) + ccp(-15, 0));
    menu->addChild(del);

    auto fadeInLbl = CCLabelBMFont::create("Fade In:", "bigFont.fnt");
    auto holdLbl = CCLabelBMFont::create("Hold:", "bigFont.fnt");
    auto fadeOutLbl = CCLabelBMFont::create("Fade Out:", "bigFont.fnt");

    fadeInLbl->setAnchorPoint(ccp(0, 0.5f));
    fadeInLbl->setScale(0.3f);

    holdLbl->setAnchorPoint(ccp(0, 0.5f));
    holdLbl->setScale(0.3f);

    fadeOutLbl->setAnchorPoint(ccp(0, 0.5f));
    fadeOutLbl->setScale(0.3f);

    sliderFadeIn = Slider::create(this, menu_selector(LabelEventCell::onSliderChanged));
    sliderHold = Slider::create(this, menu_selector(LabelEventCell::onSliderChanged));
    sliderFadeOut = Slider::create(this, menu_selector(LabelEventCell::onSliderChanged));

    sliderFadeIn->setValue(event->fadeIn / 5);
    sliderHold->setValue(event->hold / 5);
    sliderFadeOut->setValue(event->fadeOut / 5);

    sliderFadeIn->setAnchorPoint(ccp(0, 0));
    sliderFadeIn->setScale(0.4f);
    sliderFadeIn->setScaleX(0.3f);
    getChildOfType<CCSprite>(sliderFadeIn->getThumb(), 0)->setScaleX(0.4f / 0.3f);
    getChildOfType<CCSprite>(sliderFadeIn->getThumb(), 1)->setScaleX(0.4f / 0.3f);

    sliderHold->setAnchorPoint(ccp(0, 0));
    sliderHold->setScale(0.4f);
    sliderHold->setScaleX(0.3f);
    getChildOfType<CCSprite>(sliderHold->getThumb(), 0)->setScaleX(0.4f / 0.3f);
    getChildOfType<CCSprite>(sliderHold->getThumb(), 1)->setScaleX(0.4f / 0.3f);

    sliderFadeOut->setAnchorPoint(ccp(0, 0));
    sliderFadeOut->setScale(0.4f);
    sliderFadeOut->setScaleX(0.3f);
    getChildOfType<CCSprite>(sliderFadeOut->getThumb(), 0)->setScaleX(0.4f / 0.3f);
    getChildOfType<CCSprite>(sliderFadeOut->getThumb(), 1)->setScaleX(0.4f / 0.3f);

    inputFadeIn = TextInput::create(50, "Time");
    inputHold = TextInput::create(50, "Time");
    inputFadeOut = TextInput::create(50, "Time");

    inputFadeIn->setScale(0.4f);
    inputHold->setScale(0.4f);
    inputFadeOut->setScale(0.4f);

    inputFadeIn->setString(fmt::format("{:.02f}", event->fadeIn));
    inputHold->setString(fmt::format("{:.02f}", event->hold));
    inputFadeOut->setString(fmt::format("{:.02f}", event->fadeOut));

    this->addChildAtPosition(sliderFadeIn, Anchor::Left, ccp(90, 12));
    this->addChildAtPosition(sliderHold, Anchor::Left, ccp(90, 0));
    this->addChildAtPosition(sliderFadeOut, Anchor::Left, ccp(90, -12));

    this->addChildAtPosition(inputFadeIn, Anchor::Left, ccp(135, 13));
    this->addChildAtPosition(inputHold, Anchor::Left, ccp(135, 0));
    this->addChildAtPosition(inputFadeOut, Anchor::Left, ccp(135, -13));

    this->addChildAtPosition(fadeInLbl, Anchor::Left, ccp(5, 12));
    this->addChildAtPosition(holdLbl, Anchor::Left, ccp(5, 0));
    this->addChildAtPosition(fadeOutLbl, Anchor::Left, ccp(5, -12));

    this->addChildAtPosition(bg, Anchor::Center);
    this->addChild(menu);
    return true;
}

void LabelEventCell::onSliderChanged(CCObject* sender)
{
    event->fadeIn = sliderFadeIn->getThumb()->getValue() * 5;
    event->hold = sliderHold->getThumb()->getValue() * 5;
    event->fadeOut = sliderFadeOut->getThumb()->getValue() * 5;

    inputFadeIn->setString(fmt::format("{:.02f}", event->fadeIn));
    inputHold->setString(fmt::format("{:.02f}", event->hold));
    inputFadeOut->setString(fmt::format("{:.02f}", event->fadeOut));
}

void LabelEventCell::onDelete(CCObject* sender)
{
    geode::createQuickPopup("Delete Event",
        "Are you sure you want to <cr>delete</c> this <cc>event</c>?",
        "Cancel",
        "Delete",
        [this](FLAlertLayer* alert, bool right){
            if (right)
            {
                std::erase(as<LabelModule*>(module)->events, *event);

                as<EditLabelPopup*>(layer)->updateList();
            }
        }
    );
}

LabelEventCell* LabelEventCell::createWithEvent(LabelEvent* event)
{
    auto pRet = new LabelEventCell();

    if (pRet && pRet->initWithEvent(event))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}