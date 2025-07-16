#include "../../Client/Module.hpp"
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;

class UnlockAllIcons : public Module
{
    public:
        MODULE_SETUP(UnlockAllIcons)
        {
            setName("Unlock All Icons");
            setID("icon-bypass");
            setCategory("Universal");
            setDescription("This is the help text >w<");
        }
};

SUBMIT_HACK(UnlockAllIcons);

class $modify (GameManager)
{
    bool isIconUnlocked(int p0, IconType p1)
    {
        if (UnlockAllIcons::get()->getRealEnabled())
            return true;

        return GameManager::isIconUnlocked(p0, p1);
    }

    bool isColorUnlocked(int p0, UnlockType p1)
    {
        if (UnlockAllIcons::get()->getRealEnabled())
            return true;

        return GameManager::isColorUnlocked(p0, p1);
    }
};

class $modify (GameStatsManager)
{
    bool isItemUnlocked(UnlockType type, int id)
    {
        if (UnlockAllIcons::get()->getRealEnabled())
        {
            if (type == UnlockType::GJItem)
            {
                if (id == 18 || id == 19 || id == 20)
                    return true;
            }
        }

        return GameStatsManager::isItemUnlocked(type, id);
    }
};