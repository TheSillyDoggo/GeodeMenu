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

$on_game(Loaded)
{
    auto icm = IconicManager::get();

    icm->handleIncompatibility("rooot.custom-gamemode-colors");
    icm->handleIncompatibility("gdemerald.custom_icon_colors");
    icm->handleIncompatibility("capeling.coloured-wave-trail");
    icm->handleIncompatibility("weebify.separate_dual_icons");
    icm->handleIncompatibility("naxrin.progress_bar_color");
    icm->handleIncompatibility("naxrin.rgb_icons");
    icm->handleIncompatibility("asaki_zuki.same_dual_color");
    icm->handleIncompatibility("saumondeluxe.rainbow_icon");
    icm->handleIncompatibility("terma.ambienticons");
    icm->handleIncompatibility("acaruso.pride");
    icm->handleIncompatibility("the_bearodactyl.gay-wave-trail");
}