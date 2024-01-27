/*#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    //int i = 0;
    //int d = 0;

    void destroyPlayer(PlayerObject * p0, GameObject * p1)
    {
        if (!Client::GetModuleEnabled("noclip"))
            PlayLayer::destroyPlayer(p0, p1);
    }
};*/