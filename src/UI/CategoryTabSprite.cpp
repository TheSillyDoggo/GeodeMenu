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
        if (sprite = CCSprite::createWithSpriteFrameName(icon.c_str()); sprite)
        {
            sprite->setZOrder(4);
            this->addChildAtPosition(sprite, Anchor::Center);
        }
    }

    label = TransLabelBMFont::create(name.c_str(), "bigFont.fnt");

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

    if (sprite && std::string(label->getString()).empty())
        as<AnchorLayoutOptions*>(sprite->getLayoutOptions())->setOffset(CCPointZero);
}