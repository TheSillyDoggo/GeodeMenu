#include "CategoryTabSprite.hpp"

#define SPRITE_INSET 5

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
            sprite->setAnchorPoint(ccp(0, 0.5f));
            this->addChild(sprite, 4);
        }
    }

    label = TransLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setOnLabelUpdated([this]
    {
        this->setContentSize(this->getContentSize());
    });

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

            if (sprite)
            {
                sprite->setColor(ccc3(150, 150, 150));
                sprite->setOpacity(150);
            }
            break;

        case Hovered:
            label->setColor(ccc3(200, 200, 200));
            label->setOpacity(255);

            if (sprite)
            {
                sprite->setColor(ccc3(200, 200, 200));
                sprite->setOpacity(255);
            }
            break;

        case Selected:
            label->setColor(ccc3(255, 255, 255));
            label->setOpacity(255);

            if (sprite)
            {
                sprite->setColor(ccc3(255, 255, 255));
                sprite->setOpacity(255);
            }
            break;
    };
}

void CategoryTabSprite::setContentSize(const CCSize& contentSize)
{
    CCNode::setContentSize(contentSize);

    label->limitLabelWidth((this->getContentWidth() - 7.5f) / 0.5f, 0.75f, 0.1f);
    label->setScale(label->getScale() / 2);

    outline->setContentSize(contentSize * 2);
    background->setContentSize(contentSize * 2);

    if (sprite)
    {
        sprite->setPositionX(-getContentWidth() / 2 + SPRITE_INSET);
        sprite->setScale((getContentHeight() - (SPRITE_INSET * 2)) / sprite->getContentHeight());
    }

    if (sprite && std::string(label->getString()).empty())
        as<AnchorLayoutOptions*>(sprite->getLayoutOptions())->setOffset(CCPointZero);
}