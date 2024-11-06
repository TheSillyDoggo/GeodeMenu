#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (GameStatsManager)
{
    static void onModify(auto& self) {
        (void)self.setHookPriority("GameStatsManager::isItemUnlocked", 99999999);
        auto hook = self.getHook("GameStatsManager::isItemUnlocked");
        Loader::get()->queueInMainThread([hook]{
            Client::GetModule("basement-key-bypass")->addHookRaw(hook);
        });
    }

    bool isItemUnlocked(UnlockType p0, int p1)
    {
        if (p0 == UnlockType::GJItem)
        {
            if (p1 == 1 || p1 == 2 || p1 == 3)
                return true;
        }

        return GameStatsManager::isItemUnlocked(p0, p1);
    }
};