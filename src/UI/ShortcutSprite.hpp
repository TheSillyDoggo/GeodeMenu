#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    enum class ShortcutShape : uint8_t
    {
        Circle = 0,
        Square = 1,
        Hexagon = 2,
        Octagon = 3,
    };

    enum class ShortcutColour : uint8_t
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
        Bisexual = 104,
        Pansexual = 105,
        Enby = 106,
        Aromantic = 107,
        Asexual = 108,
    };

    class ShortcutSprite : public cocos2d::CCSprite
    {
        protected:
            ShortcutShape shape;
            ShortcutColour colour;
            cocos2d::CCSprite* outline = nullptr;
            cocos2d::CCSprite* fill = nullptr;

            virtual void visit();

            static std::string shapeToString(ShortcutShape shape);
            static std::string colourToString(ShortcutColour colour);

        public:
            static ShortcutSprite* create();
            static ShortcutSprite* create(ShortcutShape shape, ShortcutColour colour);

            virtual void setOpacity(GLubyte opacity);

            bool init(ShortcutShape shape, ShortcutColour colour);

            void setShape(ShortcutShape shape);
            void setColour(ShortcutColour colour);
            void setBoth(ShortcutShape shape, ShortcutColour colour);

            ShortcutShape getShape();
            ShortcutColour getColour();
    };
};