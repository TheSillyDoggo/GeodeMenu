#include "APIManager.hpp"

using namespace geode::prelude;
using namespace qolmod;

APIManager* APIManager::get()
{
    static APIManager instance;
    return &instance;
}

void APIManager::addAntiCheatMod(geode::Mod* mod)
{
    if (std::find(anticheatMods.begin(), anticheatMods.end(), mod) == anticheatMods.end())
        anticheatMods.push_back(mod);

    enabledAntiCheat = true;
}

void APIManager::removeAntiCheatMod(geode::Mod* mod)
{
    anticheatMods.erase(std::find(anticheatMods.begin(), anticheatMods.end(), mod));

    enabledAntiCheat = anticheatMods.size() != 0;
}

const std::vector<geode::Mod*> APIManager::getAntiCheatMods()
{
    return anticheatMods;
}

const bool APIManager::isAntiCheatEnabled()
{
    return enabledAntiCheat;
}