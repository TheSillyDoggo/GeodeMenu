#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/GameManager.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameStatsManager)
{
    bool isItemUnlocked(UnlockType type, int id)
    {
        if (Module::get("icon-bypass")->enabled)
        {
            if (type == UnlockType::GJItem)
            {
                if (id == 18 || id == 19 || id == 20)
                    return true;
            }
        }
        
        if (id == 16 && Module::get("music-bypass")->enabled && type == UnlockType::GJItem)
            return true;
        
        if (id == 17 && Module::get("practice-bypass")->enabled && type == UnlockType::GJItem)
            return true;
        
        return GameStatsManager::isItemUnlocked(type, id);
    }
};

class $modify (GameManager)
{
    bool isIconUnlocked(int id, IconType type)
    {
        if (Module::get("icon-bypass")->enabled)
            return true;

        return GameManager::isIconUnlocked(id, type);
    }

    bool isColorUnlocked(int id, UnlockType type)
    {
        if (Module::get("icon-bypass")->enabled)
            return true;

        return GameManager::isColorUnlocked(id, type);
    }
};