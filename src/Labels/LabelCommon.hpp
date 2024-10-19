#pragma once

#include <Geode/Geode.hpp>

enum class LabelAnchor
{
    TopLeft,
    TopCenter,
    TopRight,
    CenterRight,
    BottomRight,
    BottomCenter,
    BottomLeft,
    CenterLeft,
    Center,
};

enum class LabelEventType
{
    PlayerTookDamage,
    ClickStarted,
    ClickEnded,
    P1ClickStarted,
    P1ClickEnded,
    P2ClickStarted,
    P2ClickEnded,
};

struct LabelEvent
{
    enum LabelEventType type;
    bool enabled = false;
    cocos2d::ccColor3B colour = cocos2d::ccc3(0, 255, 0);
    float fadeIn = 0;
    float hold = 0;
    float fadeOut = 0.5f;

    friend bool operator==(const LabelEvent& a, const LabelEvent& b);

    matjson::Object save();
    void load(matjson::Object obj);
};