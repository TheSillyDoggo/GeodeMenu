#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    void toggleFlipped(bool p0, bool p1)
    {
        if (Module::get("instant-reverse")->enabled)
            p1 = true;

        if (!Module::get("no-reverse")->enabled)
            GJBaseGameLayer::toggleFlipped(p0, p1);
    }
};