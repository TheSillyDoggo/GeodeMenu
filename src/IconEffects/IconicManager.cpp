#include "IconicManager.hpp"

IconicManager* IconicManager::get()
{
    static IconicManager* instance = nullptr;

    if (!instance)
        instance = new IconicManager();

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