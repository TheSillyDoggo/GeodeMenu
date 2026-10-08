#pragma once

#include <Geode/Geode.hpp>
#include <ShortcutSprite.hpp>
#include <stdint.h>
#include <FloatingButton/FloatingButtonBase.hpp>
#include <ColourConfig.hpp>

class ShortcutVisualConfig
{
    public:
        using SpritePair = std::pair<qolmod::ShortcutShape, qolmod::ShortcutColour>;
        enum class OverlayType : uint8_t
        {
            None = 0,
            BuiltIn = 1,
            Text = 2,
            Image = 3,
        };

    protected:
        // first 5 = visibility, 6 = is movable, 7 = no off bg, 8 = no on bg, 9 = enabled, rest are just if i need them
        std::bitset<16> flags;
        std::string moduleID;
        FloatingButtonBase::AnimationType animation;
        SpritePair offSprite;
        SpritePair onSprite;
        float scale = 0.8f;
        float opacity = 0.8f;
        uint8_t group = 0;
        ColourConfig overlayColour = { cocos2d::ccc3(255, 255, 255) };
        OverlayType overlayType;
        std::string overlayBuiltinSprite;
        std::string overlayText;
        std::string overlayFont;
        std::vector<uint8_t> customOverlayData;

        static SpritePair stringToPair(std::string old);

    public:
        void loadV1(matjson::Value value, bool enabled, std::string moduleID);
        void loadV2(std::filesystem::path path);
        void saveV2(std::filesystem::path path);

        bool shouldShow();
        bool isMovable();
        bool isEnabled();

        float getScale();
        float getOpacity();

        bool hasOffBG();
        bool hasOnBG();
        SpritePair getOffBG();
        SpritePair getOnBG();
};