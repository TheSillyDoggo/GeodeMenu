#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameStatsManager)
{
    bool isItemUnlocked(UnlockType type, int id)
    {
        if (Client::GetModuleEnabled("icon-bypass"))
        {
            if (type != UnlockType::GJItem)
            {
                return true;
            }
        }
        
        if (id == 16 && Client::GetModuleEnabled("music-bypass"))
        {
            return true;
        }
        else if (id == 17 && Client::GetModuleEnabled("practice-bypass"))
        {
            return true;
        }
        else
        {
            return GameStatsManager::isItemUnlocked(type, id);
        }
    }
};