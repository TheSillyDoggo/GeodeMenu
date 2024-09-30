#include "IconOptionsLayer.h"

void IconOptionsLayer::customSetup()
{
    startFade = Mod::get()->getSavedValue<ccColor3B>(fmt::format("fadeColour1{}", icon), {0, 0, 0});
    endFade = Mod::get()->getSavedValue<ccColor3B>(fmt::format("fadeColour2{}", icon), {255, 255, 255});

    auto back = CCScale9Sprite::create("square02_001.png");
    back->setOpacity(100);
    back->setContentSize(ccp(150, 100) / 0.5f);
    back->setScale(0.5f);

    auto backSpeed = CCScale9Sprite::create("square02_001.png");
    backSpeed->setOpacity(100);
    backSpeed->setContentSize(ccp(150, 100) / 0.5f);
    backSpeed->setScale(0.5f);

    auto label1 = CCLabelBMFont::create("Fade Start", "bigFont.fnt");
    label1->setScale(0.5f);

    auto label2 = CCLabelBMFont::create("Fade End", "bigFont.fnt");
    label2->setScale(0.5f);

    auto label3 = CCLabelBMFont::create("Speed", "bigFont.fnt");
    label3->setScale(0.5f);

    spr1 = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    spr1->setContentWidth(spr1->getContentWidth() * 3);
    spr1->setColor(startFade);
    label1->setPosition(spr1->getContentSize() * 0.5f + ccp(-1.5f, 0));
    spr1->addChild(label1);

    spr2 = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    spr2->setContentWidth(spr2->getContentWidth() * 3);
    spr2->setColor(endFade);
    label2->setPosition(spr2->getContentSize() * 0.5f + ccp(-1.5f, 0));
    spr2->addChild(label2);

    auto btn1 = CCMenuItemSpriteExtra::create(spr1, this, menu_selector(IconOptionsLayer::onColour));
    btn1->setTag(1);
    auto btn2 = CCMenuItemSpriteExtra::create(spr2, this, menu_selector(IconOptionsLayer::onColour));
    btn2->setTag(2);

    input = TextInput::create(spr1->getContentWidth(), "Speed");
    input->setFilter("1234567890.");
    input->setString(fmt::format("{}", Mod::get()->getSavedValue<float>(fmt::format("icon-effect-speed_{}", icon), 1)));

    baseLayer->addChildAtPosition(back, Anchor::Center, ccp(-80, 10));
    baseLayer->addChildAtPosition(backSpeed, Anchor::Center, ccp(80, 10));
    baseLayer->addChildAtPosition(btn1, Anchor::Center, ccp(-80, 35));
    baseLayer->addChildAtPosition(btn2, Anchor::Center, ccp(-80, -15));
    baseLayer->addChildAtPosition(label3, Anchor::Center, ccp(80, 48));
    baseLayer->addChildAtPosition(input, Anchor::Center, ccp(80, 20));

    ok->m_pfnSelector = menu_selector(IconOptionsLayer::onClose); // sexy
}

void IconOptionsLayer::onColour(CCObject* sender)
{
    auto popup = ColorPickPopup::create((as<CCNode*>(sender)->getTag() == 1) ? startFade : endFade);
    popup->setTag(as<CCNode*>(sender)->getTag());
    popup->show();
    popup->setDelegate(this);
}

void IconOptionsLayer::onClose(CCObject* sender)
{
    Mod::get()->setSavedValue<ccColor3B>(fmt::format("fadeColour1{}", icon), startFade);
    Mod::get()->setSavedValue<ccColor3B>(fmt::format("fadeColour2{}", icon), endFade);
    Mod::get()->setSavedValue<float>(fmt::format("icon-effect-speed_{}", icon), numFromString<float>(input->getString()).unwrapOr(1.0f));

    SillyBaseLayer::onClose(sender);
}

void IconOptionsLayer::updateColor(cocos2d::ccColor4B const& color)
{
    if (auto popup = getChildOfType<ColorPickPopup>(CCScene::get(), 0))
    {
        auto col = ccc3(color.r, color.g, color.b);

        if (popup->getTag() == 1)
            startFade = col;
        else
            endFade = col;

        spr1->setColor(startFade);
        spr2->setColor(endFade);
    }
}