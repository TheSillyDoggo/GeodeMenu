#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

#ifndef GEODE_IS_ANDROID32
class $modify (PlayLayer)
{
    TodoReturn delayedResetLevel()
    {
        if (!Client::GetModuleEnabled("instant-restart"))
            PlayLayer::delayedResetLevel();
    }
};
#endif

class $modify (PlayerObject)
{
    void playerDestroyed(bool p0)
    {
        log::info("penis");

        log::info("unk: {}", p0);

        #ifndef GEODE_IS_ANDROID32

        if (Client::GetModuleEnabled("instant-restart") && PlayLayer::get())
            PlayLayer::get()->resetLevel();
        #endif

        if (!Client::GetModuleEnabled("no-death"))
            PlayerObject::playerDestroyed(p0);
    }
};