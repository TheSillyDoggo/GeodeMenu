#pragma once

#include <Geode/Geode.hpp>
#include "../GUI/FloatingButton/FloatingButtonBase.hpp"
#include "../Utils/ColourConfig.hpp"

struct FloatingUIButtonVisibility
{
    bool showInMenu = true;
    bool showInGame = true;
    bool showInPauseMenu = true;
    bool showInEditor = true;
    bool showInEditorPauseMenu = true;

    bool shouldShow();
};

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
    FloatingButtonBase::AnimationType animation = FloatingButtonBase::AnimationType::Shrink;
    int group = 0;
};