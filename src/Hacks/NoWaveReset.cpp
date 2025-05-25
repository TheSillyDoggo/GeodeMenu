#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    void fadeOutStreak2(float p0)
    {
        if (!Module::get("no-wave-reset")->enabled)
            PlayerObject::fadeOutStreak2(p0);
    }
};