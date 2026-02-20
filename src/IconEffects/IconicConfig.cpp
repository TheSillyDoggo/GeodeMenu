#include "IconicConfig.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

IconicConfig* IconicConfig::create(IconicGamemodeType gamemode, bool player2)
{
    auto pRet = new IconicConfig();

    std::string gm = "";

    switch (gamemode)
    {
        case IconicGamemodeType::Cube:
            gm = "cube";
            break;
        case IconicGamemodeType::Ship:
            gm = "ship";
            break;
        case IconicGamemodeType::Jetpack:
            gm = "jetpack";
            break;
        case IconicGamemodeType::Ball:
            gm = "ball";
            break;
        case IconicGamemodeType::Ufo:
            gm = "bird";
            break;
        case IconicGamemodeType::Wave:
            gm = "dart";
            break;
        case IconicGamemodeType::Robot:
            gm = "robot";
            break;
        case IconicGamemodeType::Spider:
            gm = "spider";
            break;
        case IconicGamemodeType::Swing:
            gm = "swing";
            break;
    }

    pRet->saveStr = fmt::format("iconic-{}-{}", gm, player2 ? "p2" : "p1");
    pRet->load();

    return pRet;
}

void IconicConfig::save()
{
    matjson::Value value;
    value["primary"] = primary.toJson();
    value["secondary"] = secondary.toJson();
    value["glow"] = glow.toJson();

    auto parent = Mod::get()->getSavedValue<matjson::Value>("iconic-config");
    parent[saveStr] = value;

    Mod::get()->setSavedValue<matjson::Value>("iconic-config", parent);
}

void IconicConfig::load()
{
    auto parent = Mod::get()->getSavedValue<matjson::Value>("iconic-config");
    auto comp = parent[saveStr];

    if (comp.contains("primary"))
        primary.fromJson(comp["primary"]);

    if (comp.contains("secondary"))
        secondary.fromJson(comp["secondary"]);

    if (comp.contains("glow"))
        glow.fromJson(comp["glow"]);

    save();
}

cocos2d::ccColor3B IconicConfig::getPrimary()
{
    return primary.colourForConfig(fmt::format("{}_primary", saveStr));
}

cocos2d::ccColor3B IconicConfig::getSecondary()
{
    return secondary.colourForConfig(fmt::format("{}_secondary", saveStr));
}

cocos2d::ccColor3B IconicConfig::getGlow()
{
    return glow.colourForConfig(fmt::format("{}_glow", saveStr));
}

ColourConfig IconicConfig::getPrimaryConfig()
{
    return primary;
}

ColourConfig IconicConfig::getSecondaryConfig()
{
    return secondary;
}

ColourConfig IconicConfig::getGlowConfig()
{
    return glow;
}

void IconicConfig::setPrimaryConfig(ColourConfig config)
{
    primary = config;

    save();
}

void IconicConfig::setSecondaryConfig(ColourConfig config)
{
    secondary = config;

    save();
}

void IconicConfig::setGlowConfig(ColourConfig config)
{
    glow = config;

    save();
}