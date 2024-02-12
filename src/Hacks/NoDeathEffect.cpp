#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    TodoReturn delayedResetLevel()
    {
        if (!Client::GetModuleEnabled("instant-restart"))
            PlayLayer::delayedResetLevel();
    }
};

class $modify (PlayerObject)
{
    void playerDestroyed(bool p0)
    {
        log::info("penis");

        log::info("unk: {}", p0);

        if (Client::GetModuleEnabled("instant-restart") && PlayLayer::get())
            PlayLayer::get()->resetLevel();

        if (!Client::GetModuleEnabled("no-death"))
            PlayerObject::playerDestroyed(p0);
    }
};