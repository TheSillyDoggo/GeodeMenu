#include "../../Client/Module.hpp"
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;

class PracticeMusicHack : public Module
{
    public:
        MODULE_SETUP(PracticeMusicHack)
        {
            setName("Practice Music Hack");
            setID("practice-bypass");
            setCategory("Universal");
            setDescription("Unlocks the Practice Music Sync button in the pause menu options");
        }
};

SUBMIT_HACK(PracticeMusicHack)

class $modify (GameStatsManager)
{
    bool isItemUnlocked(UnlockType type, int id)
    {
        if (PracticeMusicHack::get()->getRealEnabled())
        {
            if (type == UnlockType::GJItem && id == 17)
                return true;
        }

        return GameStatsManager::isItemUnlocked(type, id);
    }
};