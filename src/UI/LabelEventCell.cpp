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
    del->setPosition(this->getContentSize() * ccp(1, 0.5f) + ccp(-15, -5));
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

    updateSliders();

    sliderFadeIn->setAnchorPoint(ccp(0, 0));
    sliderFadeIn->setScale(0.4f);
    sliderFadeIn->setScaleX(0.3f);
    sliderFadeIn->getThumb()->getChildByType<CCSprite>(0)->setScaleX(0.4f / 0.3f);
    sliderFadeIn->getThumb()->getChildByType<CCSprite>(1)->setScaleX(0.4f / 0.3f);

    sliderHold->setAnchorPoint(ccp(0, 0));
    sliderHold->setScale(0.4f);
    sliderHold->setScaleX(0.3f);
    sliderHold->getThumb()->getChildByType<CCSprite>(0)->setScaleX(0.4f / 0.3f);
    sliderHold->getThumb()->getChildByType<CCSprite>(1)->setScaleX(0.4f / 0.3f);

    sliderFadeOut->setAnchorPoint(ccp(0, 0));
    sliderFadeOut->setScale(0.4f);
    sliderFadeOut->setScaleX(0.3f);
    sliderFadeOut->getThumb()->getChildByType<CCSprite>(0)->setScaleX(0.4f / 0.3f);
    sliderFadeOut->getThumb()->getChildByType<CCSprite>(1)->setScaleX(0.4f / 0.3f);

    inputFadeIn = TextInput::create(50, "Time");
    inputHold = TextInput::create(50, "Time");
    inputFadeOut = TextInput::create(50, "Time");

    inputFadeIn->setCommonFilter(CommonFilter::Float);
    inputHold->setCommonFilter(CommonFilter::Float);
    inputFadeOut->setCommonFilter(CommonFilter::Float);

    inputFadeIn->setCallback([this, event](const std::string& str)
    {
        event->fadeIn = numFromString<float>(str).unwrapOr(event->fadeIn);

        updateSliders();
    });

    inputHold->setCallback([this, event](const std::string& str)
    {
        event->hold = numFromString<float>(str).unwrapOr(event->hold);

        updateSliders();
    });

    inputFadeOut->setCallback([this, event](const std::string& str)
    {
        event->fadeOut = numFromString<float>(str).unwrapOr(event->fadeOut);

        updateSliders();
    });

    inputFadeIn->setScale(0.4f);
    inputHold->setScale(0.4f);
    inputFadeOut->setScale(0.4f);

    inputFadeIn->setString(fmt::format("{:.02f}", event->fadeIn));
    inputHold->setString(fmt::format("{:.02f}", event->hold));
    inputFadeOut->setString(fmt::format("{:.02f}", event->fadeOut));

    auto menuInf = CCMenu::create();
    menuInf->setContentSize(CCPointZero);

    auto btnSpr = ButtonSprite::create("Inf", "bigFont.fnt", "GJ_button_05.png");
    btnSpr->setScale(0.25f);

    auto holdInf = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(LabelEventCell::onInf));
    holdInf->setTag(1);

    auto fadeOutInf = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(LabelEventCell::onInf));
    fadeOutInf->setTag(2);

    fadeOutInf->setPositionY(-13);

    menuInf->addChild(holdInf);
    menuInf->addChild(fadeOutInf);

    std::string typeStr = "";

    if (event->type == LabelEventType::ClickStarted)
        typeStr = "Click Started";

    if (event->type == LabelEventType::ClickEnded)
        typeStr = "Click Ended";

    if (event->type == LabelEventType::P1ClickStarted)
        typeStr = "P1 Click Started";

    if (event->type == LabelEventType::P1ClickEnded)
        typeStr = "P1 Click Ended";

    if (event->type == LabelEventType::P2ClickStarted)
        typeStr = "P2 Click Started";

    if (event->type == LabelEventType::P2ClickEnded)
        typeStr = "P2 Click Ended";

    if (event->type == LabelEventType::PlayerTookDamage)
        typeStr = "Player Took Damage";

    auto typeLbl = CCLabelBMFont::create(typeStr.c_str(), "bigFont.fnt");
    typeLbl->limitLabelWidth(88, 0.3f, 0);

    auto setColourMenu = CCMenu::create();
    setColourMenu->setContentSize(CCPointZero);

    setColourBG = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    setColourBG->setScale(0.5f);
    setColourBG->setContentWidth(80);
    setColourBG->setColor(ccc3(event->colour.r, event->colour.g, event->colour.b));

    auto setColourLbl = CCLabelBMFont::create("Set", "bigFont.fnt");
    setColourLbl->setScale(0.7f);
    setColourBG->addChildAtPosition(setColourLbl, Anchor::Center, ccp(-1, 1));

    auto setColourBtn = CCMenuItemSpriteExtra::create(setColourBG, this, menu_selector(LabelEventCell::onSetColour));
    setColourMenu->addChild(setColourBtn);

    this->addChildAtPosition(sliderFadeIn, Anchor::Left, ccp(90, 12));
    this->addChildAtPosition(sliderHold, Anchor::Left, ccp(90, 0));
    this->addChildAtPosition(sliderFadeOut, Anchor::Left, ccp(90, -12));

    this->addChildAtPosition(inputFadeIn, Anchor::Left, ccp(135, 13));
    this->addChildAtPosition(inputHold, Anchor::Left, ccp(135, 0));
    this->addChildAtPosition(inputFadeOut, Anchor::Left, ccp(135, -13));

    this->addChildAtPosition(fadeInLbl, Anchor::Left, ccp(5, 12));
    this->addChildAtPosition(holdLbl, Anchor::Left, ccp(5, 0));
    this->addChildAtPosition(fadeOutLbl, Anchor::Left, ccp(5, -12));

    this->addChildAtPosition(menuInf, Anchor::Left, ccp(157, 0));
    this->addChildAtPosition(typeLbl, Anchor::TopRight, ccp(-50, -11.5f));
    this->addChildAtPosition(setColourMenu, Anchor::BottomRight, ccp(-50, 15));

    this->addChildAtPosition(bg, Anchor::Center);
    this->addChild(menu);
    return true;
}

void LabelEventCell::updateSliders()
{
    sliderFadeIn->setValue(clamp<float>(event->fadeIn / 5, 0, 1));
    sliderHold->setValue(clamp<float>(event->hold / 5, 0, 1));
    sliderFadeOut->setValue(clamp<float>(event->fadeOut / 5, 0, 1));
}

void LabelEventCell::updateInputs()
{
    inputFadeIn->setString(fmt::format("{:.02f}", event->fadeIn));
    inputHold->setString(fmt::format("{:.02f}", event->hold));
    inputFadeOut->setString(fmt::format("{:.02f}", event->fadeOut));
}

void LabelEventCell::onSliderChanged(CCObject* sender)
{
    event->fadeIn = sliderFadeIn->getThumb()->getValue() * 5;
    event->hold = sliderHold->getThumb()->getValue() * 5;
    event->fadeOut = sliderFadeOut->getThumb()->getValue() * 5;

    updateInputs();
}

void LabelEventCell::onInf(CCObject* sender)
{
    if (sender->getTag() == 0)
        event->fadeIn = -1;
    else if (sender->getTag() == 1)
        event->hold = -1;
    else
        event->fadeOut = -1;

    updateSliders();
    updateInputs();
}

void LabelEventCell::onSetColour(CCObject* sender)
{
    auto popup = ColorPickPopup::create(event->colour);
    popup->setDelegate(this);

    popup->show();
}

void LabelEventCell::updateColor(cocos2d::ccColor4B const& color)
{
    event->colour = ccc4(color.r, color.g, color.b, color.a);
    setColourBG->setColor(ccc3(event->colour.r, event->colour.g, event->colour.b));
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