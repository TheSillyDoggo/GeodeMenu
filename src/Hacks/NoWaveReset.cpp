#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    void fadeOutStreak2(float p0)
    {
        if (!Client::GetModuleEnabled("no-wave-reset"))
            PlayerObject::fadeOutStreak2(p0);
    }
};