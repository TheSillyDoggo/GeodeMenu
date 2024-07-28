#include "CategoryTabSprite.hpp"

CategoryTabSprite* CategoryTabSprite::create(CategoryTabType type, std::string name, std::string icon)
{
    auto pRet = new CategoryTabSprite();

    if (pRet && pRet->init(type, name, icon))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool CategoryTabSprite::init(CategoryTabType type, std::string name, std::string icon)
{
    if (!CCNode::init())
        return false;

    outline = CCScale9Sprite::create("GJ_square07.png");
    outline->setScale(0.5f);

    background = CCScale9Sprite::create("square02b_small.png");
    background->setColor(ccc3(0, 0, 0));
    background->setOpacity(100);
    background->setScale(0.5f);

    if (!icon.empty())
    {
        if (sprite = CCSprite::createWithSpriteFrameName(icon.c_str()))
        {
            sprite->setZOrder(4);
            this->addChildAtPosition(sprite, Anchor::Center);
        }
    }

    label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");

    this->addChildAtPosition(background, Anchor::Center);
    this->addChildAtPosition(outline, Anchor::Center);
    this->addChildAtPosition(label, Anchor::Center);
    return true;
}

void CategoryTabSprite::updateSelection(CategorySelectionType type)
{
    outline->setVisible(type == CategorySelectionType::Selected);

    switch (type)
    {
        case Deselected:
            label->setColor(ccc3(150, 150, 150));
            label->setOpacity(150);
            break;

        case Hovered:
            label->setColor(ccc3(200, 200, 200));
            label->setOpacity(255);
            break;

        case Selected:
            label->setColor(ccc3(255, 255, 255));
            label->setOpacity(255);
            break;
    };
}

void CategoryTabSprite::setContentSize(const CCSize& contentSize)
{
    CCNode::setContentSize(contentSize);

    label->limitLabelWidth(this->getContentWidth() / 0.5f, 0.75f, 0.1f);
    label->setScale(label->getScale() / 2);

    outline->setContentSize(contentSize * 2);
    background->setContentSize(contentSize * 2);

    if (sprite)
        as<AnchorLayoutOptions*>(sprite->getLayoutOptions())->setOffset(ccp(-label->getContentWidth() / 2 - sprite->getScaledContentWidth(), 0));

    if (sprite && label->getString() == "")
        as<AnchorLayoutOptions*>(sprite->getLayoutOptions())->setOffset(CCPointZero);
}

/*
auto win = Client::instance->windows[i];

auto selectedBtn = CCScale9Sprite::create("square02b_small.png");
selectedBtn->setContentSize(ccp(100, 20) / 0.5f);
selectedBtn->setColor(ccc3(0, 0, 0));
selectedBtn->setScale(0.5f);
selectedBtn->setOpacity(100);
selectedBtn->setID("selected");

auto selectedLbl = CCLabelBMFont::create(win->name.c_str(), "bigFont.fnt");
selectedLbl->setPosition(selectedBtn->getContentSize() / 2);
selectedLbl->limitLabelWidth(100 / 0.5f, 0.75f, 0.1f);
selectedLbl->setColor(selectedTab == i ? ccc3(255, 255, 255) : ccc3(150, 150, 150));
selectedLbl->setOpacity(selectedTab == i ? 255 : 150);
selectedLbl->setID("name");

if (true) // maybe make a way to turn this off oneday?
{
    auto outline = CCScale9Sprite::create("GJ_square07.png");
    outline->setContentSize(selectedBtn->getContentSize());
    outline->setPosition(outline->getContentSize() / 2);
    outline->setVisible(selectedTab == i);
    outline->setID("outline");

    selectedBtn->addChild(outline);
    outlines.push_back(outline);
}

auto unselectedBtn = CCScale9Sprite::create("square02b_small.png");
unselectedBtn->setContentSize(ccp(100, 20) / 0.5f);
unselectedBtn->setColor(ccc3(0, 0, 0));
unselectedBtn->setScale(0.5f);
unselectedBtn->setOpacity(100);
unselectedBtn->setID("unselected");

auto unselectedLbl = CCLabelBMFont::create(win->name.c_str(), "bigFont.fnt");
unselectedLbl->setPosition(unselectedBtn->getContentSize() / 2);
unselectedLbl->limitLabelWidth(100 / 0.5f, 0.75f, 0.1f);
unselectedLbl->setColor({200, 200, 200});
unselectedLbl->setID("name");

selectedBtn->addChild(selectedLbl);
unselectedBtn->addChild(unselectedLbl);
*/