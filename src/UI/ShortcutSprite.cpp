#include "ShortcutSprite.hpp"
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;
using namespace qolmod;

class $modify(MenuLayer)
{
    virtual bool init()
    {
        MenuLayer::init();

        auto node = CCNode::create();

        for (int i = 0; i < 4; i++)
        {
            for (int x = 0; x < 14; x++)
            {
                auto spr = ShortcutSprite::create((ShortcutShape)(i), x <= 7 ? (ShortcutColour)x : (ShortcutColour)(x - 8 + 100));
                spr->setPosition(ccp(50 * x, 50 * i));
                spr->setOpacity(204);
                node->addChild(spr);
            }
        }
        
        node->setPosition(getContentSize() / 2);
        this->addChild(node, 676980085);
        return true;
    }
};

ShortcutSprite* ShortcutSprite::create(ShortcutShape shape, ShortcutColour colour)
{
    auto pRet = new ShortcutSprite();
    if (pRet && pRet->init(shape, colour))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool ShortcutSprite::init(ShortcutShape shape, ShortcutColour colour)
{
    if (!CCSprite::init())
        return false;

    this->setContentSize(ccp(45, 45));
    this->shape = shape;
    this->colour = colour;

    outline = CCSprite::createWithSpriteFrameName(fmt::format("shortcut-outline_{}.png"_spr, shapeToString(shape)).c_str());
    fill = CCSprite::createWithSpriteFrameName(fmt::format("shortcut-fill_{}_{}.png"_spr, shapeToString(shape), colourToString(colour)).c_str());

    outline->setPosition(getContentSize() / 2);
    fill->setPosition(getContentSize() / 2);
    fill->setScale(1.016f);

    this->addChild(outline);
    this->addChild(fill);
    return true;
}

void ShortcutSprite::setOpacity(GLubyte opacity)
{
    CCNodeRGBA::setOpacity(opacity);

    outline->setOpacity(opacity);
    fill->setOpacity(opacity);
}

std::string ShortcutSprite::shapeToString(ShortcutShape shape)
{
    switch (shape)
    {
        case ShortcutShape::Circle: return "circle";
        case ShortcutShape::Square: return "square";
        case ShortcutShape::Hexagon: return "hexagon";
        case ShortcutShape::Octagon: return "octagon";

        default:
            return "VERY VERY BAD ERROR!!!!!!!!";
    }
}

std::string ShortcutSprite::colourToString(ShortcutColour colour)
{
    switch (colour)
    {
        case ShortcutColour::Green: return "Green";
        case ShortcutColour::Pink: return "Pink";
        case ShortcutColour::Cyan: return "Cyan";
        case ShortcutColour::Blue: return "Blue";
        case ShortcutColour::Gray: return "Gray";
        case ShortcutColour::DarkPurple: return "DarkPurple";
        case ShortcutColour::DarkAqua: return "DarkAqua";
        case ShortcutColour::Red: return "Red";
        case ShortcutColour::Lesbian: return "Lesbian";
        case ShortcutColour::Trans: return "Trans";
        case ShortcutColour::Gay: return "Gay";
        case ShortcutColour::Pride: return "Pride";
        case ShortcutColour::BiSexual: return "Bisexual";
        case ShortcutColour::PanSexual: return "Pansexual";

        default:
            return "VERY VERY BAD ERROR!!!!!!!!";
    }
}