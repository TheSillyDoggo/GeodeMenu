#include "LabelEventCell.hpp"
#include "../../GUI/SetupColourConfigUI.hpp"

LabelEventCell* LabelEventCell::create(CCSize size, LabelEvent config, SetupLabelConfigUI* ui, int index)
{
    auto pRet = new LabelEventCell();

    if (pRet && pRet->init(size, config, ui, index))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool LabelEventCell::init(CCSize size, LabelEvent config, SetupLabelConfigUI* ui, int index)
{
    if (!CCNode::init())
        return false;

    this->size = size;
    this->config = config;
    this->ui = ui;
    this->index = index;

    this->setContentSize(size);

    auto bg = geode::NineSlice::create("square02b_001.png");
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

    sliderFadeIn = BetterSlider::create(this, menu_selector(LabelEventCell::onSliderChanged));
    sliderHold = BetterSlider::create(this, menu_selector(LabelEventCell::onSliderChanged));
    sliderFadeOut = BetterSlider::create(this, menu_selector(LabelEventCell::onSliderChanged));

    sliderFadeIn->setAnchorPoint(ccp(0, 0.5f));
    sliderHold->setAnchorPoint(ccp(0, 0.5f));
    sliderFadeOut->setAnchorPoint(ccp(0, 0.5f));
    sliderFadeIn->setScale(0.5f);
    sliderHold->setScale(0.5f);
    sliderFadeOut->setScale(0.5f);
    sliderFadeIn->setContentWidth(120);
    sliderHold->setContentWidth(120);
    sliderFadeOut->setContentWidth(120);

    sliderFadeIn->setRange(0, 5);
    sliderHold->setRange(0, 5);
    sliderFadeOut->setRange(0, 5);

    sliderFadeIn->setValueRanged(config.fadeIn);
    sliderHold->setValueRanged(config.hold);
    sliderFadeOut->setValueRanged(config.fadeOut);

    inputFadeIn = TextInput::create(50, "Time");
    inputHold = TextInput::create(50, "Time");
    inputFadeOut = TextInput::create(50, "Time");

    inputFadeIn->setCommonFilter(CommonFilter::Float);
    inputHold->setCommonFilter(CommonFilter::Float);
    inputFadeOut->setCommonFilter(CommonFilter::Float);

    inputFadeIn->setCallback([this](const std::string& str)
    {
        auto value = utils::numFromString<float>(str).unwrapOr(this->config.fadeIn);

        this->ui->currentConfig.events[this->index].fadeIn = value;
        this->sliderFadeIn->setValueRanged(value);
    });

    inputHold->setCallback([this](const std::string& str)
    {
        auto value = utils::numFromString<float>(str).unwrapOr(this->config.hold);

        this->ui->currentConfig.events[this->index].hold = value;
        this->sliderHold->setValueRanged(value);
    });

    inputFadeOut->setCallback([this](const std::string& str)
    {
        auto value = utils::numFromString<float>(str).unwrapOr(this->config.fadeOut);

        this->ui->currentConfig.events[this->index].fadeOut = value;
        this->sliderFadeOut->setValueRanged(value);
    });

    inputFadeIn->setScale(0.4f);
    inputHold->setScale(0.4f);
    inputFadeOut->setScale(0.4f);

    inputFadeIn->setString(fmt::format("{:.02f}", config.fadeIn));
    inputHold->setString(fmt::format("{:.02f}", config.hold));
    inputFadeOut->setString(fmt::format("{:.02f}", config.fadeOut));

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

    if (config.type == LabelEventType::ClickStarted)
        typeStr = "Click Started";

    if (config.type == LabelEventType::ClickEnded)
        typeStr = "Click Ended";

    if (config.type == LabelEventType::P1ClickStarted)
        typeStr = "P1 Click Started";

    if (config.type == LabelEventType::P1ClickEnded)
        typeStr = "P1 Click Ended";

    if (config.type == LabelEventType::P2ClickStarted)
        typeStr = "P2 Click Started";

    if (config.type == LabelEventType::P2ClickEnded)
        typeStr = "P2 Click Ended";

    if (config.type == LabelEventType::PlayerTookDamage)
        typeStr = "Player Took Damage";

    auto typeLbl = CCLabelBMFont::create(typeStr.c_str(), "bigFont.fnt");
    typeLbl->limitLabelWidth(75, 0.3f, 0);

    auto setColourMenu = CCMenu::create();
    setColourMenu->setContentSize(CCPointZero);

    auto setColourBG = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    setColourBG->setScale(0.5f);
    setColourBG->setContentWidth(70);
    setColourBG->setColor(ccc3(config.colour.r, config.colour.g, config.colour.b));

    auto setColourLbl = CCLabelBMFont::create("Set", "bigFont.fnt");
    setColourLbl->setScale(0.7f);
    setColourBG->addChildAtPosition(setColourLbl, Anchor::Center, ccp(-1, 1));

    auto setColourBtn = CCMenuItemSpriteExtra::create(setColourBG, this, menu_selector(LabelEventCell::onSetColour));
    setColourMenu->addChild(setColourBtn);

    this->addChildAtPosition(sliderFadeIn,  Anchor::Left, ccp(60, 12));
    this->addChildAtPosition(sliderHold,    Anchor::Left, ccp(60, 0));
    this->addChildAtPosition(sliderFadeOut, Anchor::Left, ccp(60, -12));

    this->addChildAtPosition(inputFadeIn, Anchor::Left, ccp(135, 13));
    this->addChildAtPosition(inputHold, Anchor::Left, ccp(135, 0));
    this->addChildAtPosition(inputFadeOut, Anchor::Left, ccp(135, -13));

    this->addChildAtPosition(fadeInLbl, Anchor::Left, ccp(5, 12));
    this->addChildAtPosition(holdLbl, Anchor::Left, ccp(5, 0));
    this->addChildAtPosition(fadeOutLbl, Anchor::Left, ccp(5, -12));

    this->addChildAtPosition(menuInf, Anchor::Left, ccp(157, 0));
    this->addChildAtPosition(typeLbl, Anchor::TopRight, ccp(-45, -11.5f));
    this->addChildAtPosition(setColourMenu, Anchor::BottomRight, ccp(-43, 15));

    this->addChildAtPosition(bg, Anchor::Center);
    this->addChild(menu);

    return true;
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
                ui->currentConfig.events.erase(ui->currentConfig.events.begin() + index);

                ui->updateEventsUI();
            }
        }
    );
}

void LabelEventCell::onInf(CCObject* sender)
{
    if (sender->getTag() == 1)
    {
        ui->currentConfig.events[index].hold = -1;
        sliderHold->setValueRanged(-1);
        inputHold->setString("-1.00");
    }

    if (sender->getTag() == 2)
    {
        ui->currentConfig.events[index].fadeOut = -1;
        sliderFadeOut->setValueRanged(-1);
        inputFadeOut->setString("-1.00");
    }
}

void LabelEventCell::onSetColour(CCObject* sender)
{
    auto ui = SetupColourConfigUI::create([this](ColourConfig conf)
    {
        this->ui->currentConfig.events[index].colour = ccc4BFromccc4F(ccc4FFromccc3B(conf.customColour));

        this->ui->updateEventsUI();
    }, false);

    ui->setStartConfig({ ccc3(config.colour.r, config.colour.g, config.colour.b) });
    ui->setDefaultConfig({ ccc3(0, 255, 0) });
    ui->setPreviewChannel("");
    ui->show();
}

void LabelEventCell::onSliderChanged(CCObject* sender)
{
    if (sender == sliderFadeIn)
        ui->currentConfig.events[index].fadeIn = sliderFadeIn->getValueRanged();
    
    if (sender == sliderHold)
        ui->currentConfig.events[index].hold = sliderHold->getValueRanged();

    if (sender == sliderFadeOut)
        ui->currentConfig.events[index].fadeOut = sliderFadeOut->getValueRanged();

    inputFadeIn->setString(fmt::format("{:.02f}",  ui->currentConfig.events[index].fadeIn));
    inputHold->setString(fmt::format("{:.02f}",    ui->currentConfig.events[index].hold));
    inputFadeOut->setString(fmt::format("{:.02f}", ui->currentConfig.events[index].fadeOut));
}