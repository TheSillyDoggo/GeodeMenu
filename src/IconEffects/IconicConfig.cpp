#include "IconicConfig.hpp"
#include <Geode/Geode.hpp>
#include "IconicManager.hpp"

using namespace geode::prelude;

IconicConfig* IconicConfig::create(IconicGamemodeType gamemode, bool player2)
{
    auto pRet = new IconicConfig();

    std::string gm = "";
    pRet->gamemode = gamemode;
    pRet->player2 = player2;

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
    value["trail"] = trail.toJson();
    value["ghost"] = ghost.toJson();
    value["wave-trail"] = waveTrail.toJson();

    value["modes"]["primary"] = primaryEnabled;
    value["modes"]["secondary"] = secondaryEnabled;
    value["modes"]["glow"] = glowEnabled;
    value["modes"]["trail"] = trailEnabled;
    value["modes"]["ghost"] = ghostEnabled;
    value["modes"]["wave-trail"] = waveTrailEnabled;

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

    if (comp.contains("trail"))
        trail.fromJson(comp["trail"]);

    if (comp.contains("ghost"))
        ghost.fromJson(comp["ghost"]);

    if (comp.contains("wave-trail"))
        waveTrail.fromJson(comp["wave-trail"]);

    if (comp.contains("modes"))
    {
        auto modes = comp["modes"];

        primaryEnabled = modes["primary"].asBool().unwrapOr(false);
        secondaryEnabled = modes["secondary"].asBool().unwrapOr(false);
        glowEnabled = modes["glow"].asBool().unwrapOr(false);
        trailEnabled = modes["trail"].asBool().unwrapOr(false);
        ghostEnabled = modes["ghost"].asBool().unwrapOr(false);
        waveTrailEnabled = modes["wave-trail"].asBool().unwrapOr(false);
    }

    save();
}

cocos2d::ccColor3B IconicConfig::getPrimary()
{
    auto gm = gamemode;

    if (!IconicManager::get()->getSeperateColours())
        gm = IconicGamemodeType::Cube;

    if (player2)
    {
        switch (IconicManager::get()->getDualMode())
        {
            case IconicDualMode::Invert:
                return IconicManager::get()->getConfig(gm, false)->getSecondary();

            case IconicDualMode::Same:
                return IconicManager::get()->getConfig(gm, false)->getPrimary();

            case IconicDualMode::Seperate:
            default:
                break;
        }
    }

    if (!IconicManager::get()->getSeperateColours() && gamemode != IconicGamemodeType::Cube)
        return IconicManager::get()->getConfig(gm, player2)->getPrimary();

    return primary.colourForConfig(fmt::format("{}_primary", saveStr));
}

cocos2d::ccColor3B IconicConfig::getSecondary()
{
    auto gm = gamemode;

    if (!IconicManager::get()->getSeperateColours())
        gm = IconicGamemodeType::Cube;

    if (player2)
    {
        switch (IconicManager::get()->getDualMode())
        {
            case IconicDualMode::Invert:
                return IconicManager::get()->getConfig(gm, false)->getPrimary();

            case IconicDualMode::Same:
                return IconicManager::get()->getConfig(gm, false)->getSecondary();

            case IconicDualMode::Seperate:
            default:
                break;
        }
    }

    if (!IconicManager::get()->getSeperateColours() && gamemode != IconicGamemodeType::Cube)
        return IconicManager::get()->getConfig(gm, player2)->getSecondary();

    return secondary.colourForConfig(fmt::format("{}_secondary", saveStr));
}

cocos2d::ccColor3B IconicConfig::getGlow()
{
    auto gm = gamemode;

    if (!IconicManager::get()->getSeperateColours())
        gm = IconicGamemodeType::Cube;

    if (player2)
    {
        switch (IconicManager::get()->getDualMode())
        {
            case IconicDualMode::Invert:
            case IconicDualMode::Same:
                return IconicManager::get()->getConfig(gm, false)->getGlow();

            case IconicDualMode::Seperate:
            default:
                break;
        }
    }

    if (!IconicManager::get()->getSeperateColours() && gamemode != IconicGamemodeType::Cube)
        return IconicManager::get()->getConfig(gm, player2)->getGlow();

    return glow.colourForConfig(fmt::format("{}_glow", saveStr));
}

cocos2d::ccColor3B IconicConfig::getTrail()
{
    auto gm = gamemode;

    if (!IconicManager::get()->getSeperateColours())
        gm = IconicGamemodeType::Cube;

    if (player2)
    {
        switch (IconicManager::get()->getDualMode())
        {
            case IconicDualMode::Invert:
            case IconicDualMode::Same:
                return IconicManager::get()->getConfig(gm, false)->getTrail();

            case IconicDualMode::Seperate:
            default:
                break;
        }
    }

    if (!IconicManager::get()->getSeperateColours() && gamemode != IconicGamemodeType::Cube)
        return IconicManager::get()->getConfig(gm, player2)->getTrail();

    return trail.colourForConfig(fmt::format("{}_trail", saveStr));
}

cocos2d::ccColor3B IconicConfig::getGhost()
{
    auto gm = gamemode;

    if (!IconicManager::get()->getSeperateColours())
        gm = IconicGamemodeType::Cube;

    if (player2)
    {
        switch (IconicManager::get()->getDualMode())
        {
            case IconicDualMode::Invert:
            case IconicDualMode::Same:
                return IconicManager::get()->getConfig(gm, false)->getGhost();

            case IconicDualMode::Seperate:
            default:
                break;
        }
    }

    if (!IconicManager::get()->getSeperateColours() && gamemode != IconicGamemodeType::Cube)
        return IconicManager::get()->getConfig(gm, player2)->getGhost();

    return ghost.colourForConfig(fmt::format("{}_ghost", saveStr));
}

cocos2d::ccColor3B IconicConfig::getWaveTrail()
{
    auto gm = gamemode;

    if (!IconicManager::get()->getSeperateColours())
        gm = IconicGamemodeType::Dart;

    if (player2)
    {
        switch (IconicManager::get()->getDualMode())
        {
            case IconicDualMode::Invert:
            case IconicDualMode::Same:
                return IconicManager::get()->getConfig(gm, false)->getWaveTrail();

            case IconicDualMode::Seperate:
            default:
                break;
        }
    }

    if (!IconicManager::get()->getSeperateColours() && gamemode != IconicGamemodeType::Dart)
        return IconicManager::get()->getConfig(gm, player2)->getWaveTrail();

    return waveTrail.colourForConfig(fmt::format("{}_wave", saveStr));
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

ColourConfig IconicConfig::getTrailConfig()
{
    return trail;
}

ColourConfig IconicConfig::getGhostConfig()
{
    return ghost;
}

ColourConfig IconicConfig::getWaveTrailConfig()
{
    return waveTrail;
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

void IconicConfig::setTrailConfig(ColourConfig config)
{
    trail = config;

    save();
}

void IconicConfig::setGhostConfig(ColourConfig config)
{
    ghost = config;

    save();
}

void IconicConfig::setWaveTrailConfig(ColourConfig config)
{
    waveTrail = config;

    save();
}