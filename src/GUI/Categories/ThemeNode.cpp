#include "ThemeNode.hpp"
#include "../BackgroundSprite.hpp"
#include "../AndroidUI.hpp"

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
    animBG->setContentSize(ccp((getContentWidth() - 2.5f * 3) / 2, getContentHeight() - 30 + 2.5f / 2) / 0.5f);
    animBG->setScale(0.5f);
    animBG->setColor(ccc3(0, 0, 0));
    animBG->setOpacity(100);

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

    colourMenu->updateLayout();
    updateColourSprite();

    this->addChildAtPosition(colourBG, Anchor::TopLeft, ccp(2.5f, 0));
    this->addChildAtPosition(animBG, Anchor::TopRight, ccp(-2.5f, 0));
    this->addChildAtPosition(colourMenu, Anchor::TopLeft, ccp(5, -2.5f));
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

void ThemeNode::onChangeColour(CCObject* sender)
{
    Mod::get()->setSavedValue<int>("theme", sender->getTag());

    if (AndroidUI::get())
    {
        AndroidUI::get()->bg->setTheme(sender->getTag());
    }

    updateColourSprite();
}

void ThemeNode::updateColourSprite()
{
    for (auto btn : colourBtns)
    {
        btn.second->setEnabled(btn.first != Mod::get()->getSavedValue<int>("theme", -6));
        static_cast<BackgroundSprite*>(btn.second->getNormalImage())->setColour(btn.first == Mod::get()->getSavedValue<int>("theme", -6) ? ccc3(255, 255, 255) : ccc3(125, 125, 125));
    }
}