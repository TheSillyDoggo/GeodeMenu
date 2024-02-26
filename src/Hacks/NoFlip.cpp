#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    TodoReturn toggleFlipped(bool p0, bool p1)
    {
        if (!Client::GetModuleEnabled("no-reverse"))
            GJBaseGameLayer::toggleFlipped(p0, p1);
    }
};