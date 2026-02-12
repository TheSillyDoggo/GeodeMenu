#pragma once

#include <Geode/Geode.hpp>
#include "LabelAnchor.hpp"

enum class LabelType
{
    Text = 0,
    KeyChecker = 1,
    Image = 2,
};

struct LabelConfig
{
    std::string displayName = "";
    std::string formatString = "";
    float scale = 1.0f;
    float opacity = 1.0f;
    std::string font = "bigFont.fnt";
    LabelAnchor anchor = LabelAnchor::TopLeft;
    cocos2d::CCPoint offset = cocos2d::CCPointZero;
    LabelType type = LabelType::Text;
    bool cheatIndicator = false;
    bool noclipOnly = false;
    bool visible = true;
    std::vector<LabelEvent> events = {};
    // Image
    std::string imageLocation = "";

    static LabelConfig createFromObject(matjson::Value obj);
    matjson::Value save();
};