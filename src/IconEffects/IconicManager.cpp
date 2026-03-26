#include "IconicManager.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

IconicManager* IconicManager::get()
{
    static IconicManager* instance = nullptr;

    if (!instance)
    {
        instance = new IconicManager();
        instance->setup();
    }

    return instance;
}

IconicConfig* IconicManager::getConfig(IconicGamemodeType gamemode, bool player2)
{
    if (!configs.contains(gamemode))
    {
        configs[gamemode] = std::make_pair(IconicConfig::create(gamemode, false), IconicConfig::create(gamemode, true));
    }

    if (!player2)
        return configs[gamemode].first;
    else
        return configs[gamemode].second;
}

bool IconicManager::getSeperateColours()
{
    return seperate;
}

void IconicManager::setSeperateColours(bool v)
{
    seperate = v;

    auto parent = Mod::get()->getSavedValue<matjson::Value>("iconic-config");
    parent["use-seperate-colours"] = v;

    Mod::get()->setSavedValue<matjson::Value>("iconic-config", parent);
}

void IconicManager::setup()
{
    auto parent = Mod::get()->getSavedValue<matjson::Value>("iconic-config");
    
    seperate = parent["use-seperate-colours"].asBool().unwrapOr(seperate);
    dualMode = (IconicDualMode)parent["dual-mode"].asInt().unwrapOr(Mod::get()->setSavedValue<bool>("same-dual", false) ? (int)IconicDualMode::Same : (int)IconicDualMode::Seperate);
    
    fineOutline = Loader::get()->getLoadedMod("alphalaneous.fine_outline");

    handleIncompatibility("rooot.custom-gamemode-colors");
    handleIncompatibility("gdemerald.custom_icon_colors");
    handleIncompatibility("capeling.coloured-wave-trail");
    handleIncompatibility("weebify.separate_dual_icons");
    handleIncompatibility("naxrin.progress_bar_color");
    handleIncompatibility("naxrin.rgb_icons");
    handleIncompatibility("asaki_zuki.same_dual_color");
    handleIncompatibility("saumondeluxe.rainbow_icon");
    handleIncompatibility("terma.ambienticons");
    handleIncompatibility("acaruso.pride");
    handleIncompatibility("the_bearodactyl.gay-wave-trail");
}

IconicDualMode IconicManager::getDualMode()
{
    return dualMode;
}

void IconicManager::setDualMode(IconicDualMode mode)
{
    this->dualMode = mode;

    auto parent = Mod::get()->getSavedValue<matjson::Value>("iconic-config");
    parent["dual-mode"] = (int)mode;

    Mod::get()->setSavedValue<matjson::Value>("iconic-config", parent);
}

bool IconicManager::areIncompatibleModsLoaded()
{
    return getIncompatibleMods().size() > 0;
}

std::vector<geode::Mod*> IconicManager::getIncompatibleMods()
{
    return incompatibleMods;
}

void IconicManager::handleIncompatibility(std::string id)
{
    if (auto mod = Loader::get()->getLoadedMod(id))
        incompatibleMods.push_back(mod);
}

bool IconicManager::isFineOutlineLoaded()
{
    return fineOutline;
}

cocos2d::ccColor3B IconicManager::getFineOutineColour()
{
    return ccWHITE;
}

$on_game(Loaded)
{
    auto icm = IconicManager::get();
}