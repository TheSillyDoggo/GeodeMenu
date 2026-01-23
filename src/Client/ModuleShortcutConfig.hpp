#pragma once

#include <Geode/Geode.hpp>
#include "../GUI/FloatingButton/FloatingUIButton.hpp"
#include "../Utils/ColourConfig.hpp"

struct ModuleShortcutConfig
{
    std::string shortcutOverlay = "";
    std::string bgOffSprite = "geode.loader/baseCircle_Medium_Gray.png";
    std::string bgOnSprite = "geode.loader/baseCircle_Medium_Green.png";
    FloatingUIButtonVisibility visibility = {};
    float scale = 0.8f;
    float opacity = 0.8f;
    bool isMovable = true;
    ColourConfig colour = { cocos2d::ccc3(255, 255, 255) };
};