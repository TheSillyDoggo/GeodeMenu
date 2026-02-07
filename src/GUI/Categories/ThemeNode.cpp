#include "ThemeNode.hpp"
#include "../BackgroundSprite.hpp"
#include "../ExtraThemeSettingsUI.hpp"
#include "../AndroidUI.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"
#include "../BetterAlertLayer.hpp"

bool ThemeNode::init()
{
    if (!CategoryNode::init())
        return false;

    colourBG = CCScale9Sprite::create("square02b_small.png");
    colourBG->setAnchorPoint(ccp(0, 1));
    colourBG->setContentSize(ccp((getContentWidth() - 2.5f * 3) / 2, getContentHeight() - 30 + 2.5f / 2) / 0.5f);
    colourBG->setScale(0.5f);
    colourBG->setColor(ccc3(0, 0, 0));
    colourBG->setOpacity(100);

    auto animBG = CCScale9Sprite::create("square02b_small.png");
    animBG->setAnchorPoint(ccp(1, 1));
    animBG->setContentSize(ccp((getContentWidth() - 2.5f * 3) / 2, (getContentHeight() - 30 + 2.5f / 2) - (30 + 28 + 2.5f)) / 0.5f);
    animBG->setScale(0.5f);
    animBG->setColor(ccc3(0, 0, 0));
    animBG->setOpacity(100);

    auto miscBG = CCScale9Sprite::create("square02b_small.png");
    miscBG->setAnchorPoint(ccp(1, 0));
    miscBG->setContentSize(ccp((getContentWidth() - 2.5f * 3) / 2, 30 + 28) / 0.5f);
    miscBG->setScale(0.5f);
    miscBG->setColor(ccc3(0, 0, 0));
    miscBG->setOpacity(100);

    auto colourMenu = CCMenu::create();
    colourMenu->setAnchorPoint(ccp(0, 1));
    colourMenu->setScale(0.695f);
    colourMenu->setContentSize(ccp(250, 330));
    colourMenu->setLayout(AxisLayout::create(Axis::Column)->setCrossAxisOverflow(false)->setAutoScale(false)->setGrowCrossAxis(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisAlignment(AxisAlignment::Start)->setCrossAxisAlignment(AxisAlignment::Start)->setAxisReverse(true)->setCrossAxisReverse(true));

    for (int i = 1; i < 6; i++)
    {
        colourMenu->addChild(addColourBtn(i));
    }

    for (int i = 4; i < 6 + 1; i++)
    {
        colourMenu->addChild(addColourBtn(-i));
    }

    auto gradientBtn = addColourBtn(-1);
    colourMenu->addChild(gradientBtn);
    colourMenu->addChild(addColourBtn(-2));
    colourMenu->addChild(addColourBtn(-7));

    btnSpr = BetterButtonSprite::create(ccp(96, 35), "", "goldFont.fnt", "GJ_button_04.png");
    auto btnOptions = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(ThemeNode::onChangeThemeOptions));
    btnOptions->setContentSize(gradientBtn->getContentSize());
    btnOptions->getNormalImage()->setPosition(btnOptions->getContentSize() / 2);

    colourMenu->addChild(btnOptions);

    colourMenu->updateLayout();
    updateColourSprite();

    auto animMenu = CCMenu::create();
    animMenu->setAnchorPoint(ccp(1, 1));
    animMenu->setContentSize(ccp((animBG->getScaledContentWidth() - 5.0f) / 0.8f, 250));
    animMenu->setScale(0.8f);
    animMenu->ignoreAnchorPointForPosition(false);
    animMenu->setLayout(AxisLayout::create(Axis::Column)->setCrossAxisOverflow(false)->setAutoScale(false)->setGrowCrossAxis(false)->setAxisAlignment(AxisAlignment::End)->setCrossAxisAlignment(AxisAlignment::Start)->setCrossAxisAlignment(AxisAlignment::Start)->setAxisReverse(true));

    for (size_t i = 0; i < 6 + 1; i++)
    {
        animMenu->addChild(addAnimBtn((MenuAnimation)i));
    }

    auto animPreviewMenu = CCMenu::create();
    animPreviewMenu->setContentSize(ccp(0, 0));
    animPreviewMenu->setScale(1.25f);

    auto animPreviewBtn = CCMenuItemSpriteExtra::create(CCSprite::create("preview.png"_spr), this, menu_selector(ThemeNode::onPreviewAnim));
    animPreviewBtn->setPosition(ccp(70, 10));
    animPreviewMenu->addChild(animPreviewBtn);

    animMenu->addChildAtPosition(animPreviewMenu, Anchor::BottomRight, ccp(0, 0));

    animMenu->updateLayout();
    updateAnimSprite();

    blurBG = BlurMenuBG::get()->getNode();
    colonThree = UseColonThreeButton::get()->getNode();

    this->addChildAtPosition(colourBG, Anchor::TopLeft, ccp(2.5f, 0));
    this->addChildAtPosition(animBG, Anchor::TopRight, ccp(-2.5f, 0));
    this->addChildAtPosition(miscBG, Anchor::BottomRight, ccp(-2.5f, 2.5f + 1.25f));
    this->addChildAtPosition(blurBG, Anchor::BottomRight, ccp(-86.5f, 17.5f + 1.25f));
    this->addChildAtPosition(colonThree, Anchor::BottomRight, ccp(-86.5f, 17.5f + 28 + 1.25f));
    this->addChildAtPosition(colourMenu, Anchor::TopLeft, ccp(5, -2.5f));
    this->addChildAtPosition(animMenu, Anchor::TopRight, ccp(-5, -2.5f));
    return true;
}

CCMenuItemSpriteExtra* ThemeNode::addColourBtn(int colour)
{
    float width = ((colourBG->getScaledContentWidth() - (2.5f * 3)) / 2) / 0.695f;

    auto spr = BackgroundSprite::create();
    spr->setTheme(colour);
    spr->setContentSize(ccp(width, 50) / 0.8f);
    spr->setScale(0.8f);
    spr->updateLayout();

    auto spr2 = BackgroundSprite::create();
    spr2->setTheme(colour);
    spr2->setColour(ccc3(200, 200, 200));
    spr2->setContentSize(ccp(width, 50) / 0.8f);
    spr2->setScale(0.8f);
    spr2->updateLayout();

    auto btn = CCMenuItemSpriteExtra::create(spr, spr2, this, menu_selector(ThemeNode::onChangeColour));
    btn->m_scaleMultiplier = 1;
    btn->setTag(colour);
    colourBtns.emplace(colour, btn);

    return btn;
}

CCMenuItemSpriteExtra* ThemeNode::addAnimBtn(MenuAnimation anim)
{
    auto size = ccp(200, 25);

    auto spr = getAnimContainer(anim);
    auto spr2 = getAnimContainer(anim);
    static_cast<CCNodeRGBA*>(spr2->getChildByTag(0))->setColor(ccc3(200, 200, 200));
    static_cast<AdvLabelBMFont*>(spr2->getChildByTag(1))->setColor(ccc3(200, 200, 200));

    auto btn = CCMenuItemSpriteExtra::create(spr, spr2, this, menu_selector(ThemeNode::onChangeAnim));
    btn->m_scaleMultiplier = 1;
    btn->setTag((int)anim);

    btn->setContentSize(size);
    spr->setPosition(ccp(13, size.y / 2));
    spr2->setPosition(ccp(13, size.y / 2));

    animBtns.emplace(anim, btn);

    return btn;
}

CCNode* ThemeNode::getAnimContainer(MenuAnimation anim)
{
    auto sprContainer = CCNode::create();
    CCSprite* spr = nullptr;
    AdvLabelBMFont* label = nullptr;

    switch (anim)
    {
        case MenuAnimation::None:
            spr = CCSprite::create("none.png"_spr);
            label = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("animation/none"), "bigFont.fnt");
            break;

        case MenuAnimation::FromTop:
            spr = CCSprite::create("fromside.png"_spr);
            spr->setRotation(90);
            label = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("animation/from-top"), "bigFont.fnt");
            break;

        case MenuAnimation::FromBottom:
            spr = CCSprite::create("fromside.png"_spr);
            spr->setRotation(270);
            label = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("animation/from-bottom"), "bigFont.fnt");
            break;

        case MenuAnimation::FromLeft:
            spr = CCSprite::create("fromside.png"_spr);
            label = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("animation/from-left"), "bigFont.fnt");
            break;

        case MenuAnimation::FromRight:
            spr = CCSprite::create("fromside.png"_spr);
            spr->setRotation(180);
            label = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("animation/from-right"), "bigFont.fnt");
            break;

        case MenuAnimation::Scale:
            spr = CCSprite::create("scale.png"_spr);
            label = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("animation/scale-in"), "bigFont.fnt");
            break;

        case MenuAnimation::FadeIn:
            spr = CCSprite::create("fade.png"_spr);
            label = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString("animation/fade-in"), "bigFont.fnt");
            break;
    }

    spr->setScale(0.75f);
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.55f);
    label->setPositionX(18);

    spr->setTag(0);
    label->setTag(1);

    sprContainer->addChild(spr);
    sprContainer->addChild(label);

    return sprContainer;
}

void ThemeNode::onChangeColour(CCObject* sender)
{
    Mod::get()->setSavedValue<int>("theme", sender->getTag());

    if (AndroidUI::get())
    {
        AndroidUI::get()->bg->setTheme(sender->getTag());
    }

    updateColourSprite();
}

void ThemeNode::onChangeAnim(CCObject* sender)
{
    Mod::get()->setSavedValue<int>("menu-animation", sender->getTag());

    updateAnimSprite();
}

void ThemeNode::onPreviewAnim(CCObject* sender)
{
    auto anim = (MenuAnimation)Mod::get()->getSavedValue<int>("menu-animation", (int)MenuAnimation::Scale);

    if (anim == MenuAnimation::None)
        BetterAlertLayer::createWithLocalisation("animation/preview-error-none/title", "animation/preview-error-none/text", "ui/ok-button")->show();

    if (AndroidUI::get())
        AndroidUI::get()->runAnimation(anim);
}

void ThemeNode::onChangeThemeOptions(CCObject* sender)
{
    auto theme = Mod::get()->getSavedValue<int>("theme", -6);

    ExtraThemeSettingsUI::create(theme == -7)->show();
}

void ThemeNode::updateColourSprite()
{
    auto theme = Mod::get()->getSavedValue<int>("theme", -6);

    for (auto btn : colourBtns)
    {
        btn.second->setEnabled(btn.first != theme);
        static_cast<BackgroundSprite*>(btn.second->getNormalImage())->setColour(btn.first == theme ? ccc3(255, 255, 255) : ccc3(125, 125, 125));
    }

    auto key = theme == -7 ? "ui/themes-edit-image-button" : "ui/themes-edit-button";
    btnSpr->setString(LocalisationManager::get()->getLocalisedString(key));
}

void ThemeNode::updateAnimSprite()
{
    for (auto btn : animBtns)
    {
        auto col = (int)btn.first == Mod::get()->getSavedValue<int>("menu-animation", (int)MenuAnimation::Scale) ? ccc3(255, 255, 255) : ccc3(125, 125, 125);

        btn.second->setEnabled((int)btn.first != Mod::get()->getSavedValue<int>("menu-animation", (int)MenuAnimation::Scale));
        static_cast<CCNodeRGBA*>(btn.second->getNormalImage()->getChildByTag(0))->setColor(col);
        static_cast<AdvLabelBMFont*>(btn.second->getNormalImage()->getChildByTag(1))->setColor(col);
    }
}