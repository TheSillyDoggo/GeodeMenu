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
    dualMode = (IconicDualMode)parent["dual-mode"].asInt().unwrapOr((int)dualMode);
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