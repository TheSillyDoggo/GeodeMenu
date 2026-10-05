#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    enum class ShortcutShape
    {
        Circle = 0,
        Square = 1,
        Hexagon = 2,
        Octagon = 3,
    };

    enum class ShortcutColour
    {
        Green = 0,
        Pink = 1,
        Cyan = 2,
        Blue = 3,
        Gray = 4,
        DarkPurple = 5,
        DarkAqua = 6,
        Red = 7,
        Lesbian = 100,
        Trans = 101,
        Gay = 102,
        Pride = 103,
        BiSexual = 104,
        PanSexual = 105,
    };

    class ShortcutSprite : public cocos2d::CCSprite
    {
        protected:
            ShortcutShape shape;
            ShortcutColour colour;
            cocos2d::CCSprite* outline = nullptr;
            cocos2d::CCSprite* fill = nullptr;

            static std::string shapeToString(ShortcutShape shape);
            static std::string colourToString(ShortcutColour colour);

        public:
            static ShortcutSprite* create(ShortcutShape shape, ShortcutColour colour);

            virtual void setOpacity(GLubyte opacity);

            bool init(ShortcutShape shape, ShortcutColour colour);
    };
};