#include "../../Client/Module.hpp"
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;

class MusicUnlockHack : public Module
{
    public:
        MODULE_SETUP(MusicUnlockHack)
        {
            setName("Music Customiser Bypass");
            setID("music-bypass");
            setCategory("Universal");
            setDescription("Unlocks the menu and practice music buttons");
        }
};

SUBMIT_HACK(MusicUnlockHack)

class $modify (GameStatsManager)
{
    bool isItemUnlocked(UnlockType type, int id)
    {
        if (MusicUnlockHack::get()->getRealEnabled())
        {
            if (type == UnlockType::GJItem && id == 16)
                return true;
        }

        return GameStatsManager::isItemUnlocked(type, id);
    }
};