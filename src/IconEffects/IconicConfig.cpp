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

#define PRE_CHECK($gamemode, $func) \
if (player2 && IconicManager::get()->getDualMode() == IconicDualMode::Same) \
    return IconicManager::get()->getConfig(gamemode, false)->$func(); \
if (!IconicManager::get()->getSeperateColours() && gamemode != IconicGamemodeType::$gamemode) \
    return IconicManager::get()->getConfig(IconicGamemodeType::$gamemode, player2)->$func();

#define PRE_CHECK_DART() \
if (gamemode != IconicGamemodeType::Dart) \
    return IconicManager::get()->getConfig(IconicGamemodeType::Dart, player2)->getWaveTrail();

cocos2d::ccColor3B IconicConfig::getPrimary(bool ignoreP2)
{
    PRE_CHECK(Cube, getPrimary)

    if (!primaryEnabled)
        return getDefault(IconicEffectType::Primary);

    return primary.colourForConfig(fmt::format("{}_primary", saveStr));
}

cocos2d::ccColor3B IconicConfig::getSecondary(bool ignoreP2)
{
    PRE_CHECK(Cube, getSecondary)

    if (!secondaryEnabled)
        return getDefault(IconicEffectType::Secondary);

    return secondary.colourForConfig(fmt::format("{}_secondary", saveStr));
}

cocos2d::ccColor3B IconicConfig::getGlow()
{
    PRE_CHECK(Cube, getGlow)

    if (!glowEnabled)
        return getDefault(IconicEffectType::Glow);

    return glow.colourForConfig(fmt::format("{}_glow", saveStr));
}

cocos2d::ccColor3B IconicConfig::getTrail()
{
    PRE_CHECK(Cube, getTrail)

    if (!trailEnabled)
        return getDefault(IconicEffectType::Trail);

    return trail.colourForConfig(fmt::format("{}_trail", saveStr));
}

cocos2d::ccColor3B IconicConfig::getGhost()
{
    PRE_CHECK(Cube, getGhost)

    if (!ghostEnabled)
        return getDefault(IconicEffectType::Ghost);

    return ghost.colourForConfig(fmt::format("{}_ghost", saveStr));
}

cocos2d::ccColor3B IconicConfig::getWaveTrail()
{
    PRE_CHECK(Dart, getWaveTrail)
    PRE_CHECK_DART()

    if (!waveTrailEnabled)
        return getDefault(IconicEffectType::WaveTrail);

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

bool IconicConfig::getUseOverride(IconicEffectType type)
{
    switch (type)
    {
        case IconicEffectType::Primary:
            return primaryEnabled;

        case IconicEffectType::Secondary:
            return secondaryEnabled;

        case IconicEffectType::Glow:
            return glowEnabled;

        case IconicEffectType::Trail:
            return trailEnabled;

        case IconicEffectType::Ghost:
            return ghostEnabled;

        case IconicEffectType::WaveTrail:
            if (gamemode != IconicGamemodeType::Dart)
                return IconicManager::get()->getConfig(IconicGamemodeType::Dart, player2)->getUseOverride(IconicEffectType::WaveTrail);

            return waveTrailEnabled;
    }

    return false;
}

void IconicConfig::setUseOverride(IconicEffectType type, bool v)
{
    switch (type)
    {
        case IconicEffectType::Primary:
            primaryEnabled = v;
            break;

        case IconicEffectType::Secondary:
            secondaryEnabled = v;
            break;

        case IconicEffectType::Glow:
            glowEnabled = v;
            break;

        case IconicEffectType::Trail:
            trailEnabled = v;
            break;

        case IconicEffectType::Ghost:
            ghostEnabled = v;
            break;

        case IconicEffectType::WaveTrail:
            if (gamemode != IconicGamemodeType::Dart)
            {
                auto conf = IconicManager::get()->getConfig(IconicGamemodeType::Dart, player2);
                conf->setUseOverride(IconicEffectType::WaveTrail, v);
                return;
            }
            waveTrailEnabled = v;
            break;
    }

    save();
}

cocos2d::ccColor3B IconicConfig::getDefault(IconicEffectType type)
{
    auto gm = GameManager::get();
    bool secondary = player2;

    switch (type)
    {
        case IconicEffectType::Primary:
            return gm->colorForIdx(secondary ? gm->m_playerColor2.value() : gm->m_playerColor.value());

        case IconicEffectType::Secondary:
            return gm->colorForIdx(!secondary ? gm->m_playerColor2.value() : gm->m_playerColor.value());

        case IconicEffectType::Glow:
            return gm->colorForIdx(gm->m_playerGlowColor.value());

        case IconicEffectType::Trail:
            return gm->colorForIdx(secondary ? gm->m_playerColor2.value() : gm->m_playerColor.value());

        case IconicEffectType::Ghost:
            return gm->colorForIdx(secondary ? gm->m_playerColor2.value() : gm->m_playerColor.value());

        case IconicEffectType::WaveTrail:
            if (gm->getGameVariable(GameVar::SwitchWaveTrailCol))
                secondary = !secondary;

            return gm->colorForIdx(secondary ? gm->m_playerColor2.value() : gm->m_playerColor.value());

        default:
            return ccc3(255, 0, 220);
    }
}