#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GameObject.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GJBaseGameLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    void toggleFlipped(bool p0, bool p1)
    {
        if (Client::GetModuleEnabled("instant-reverse"))
            p1 = true;

        if (!Client::GetModuleEnabled("no-reverse"))
            GJBaseGameLayer::toggleFlipped(p0, p1);
    }
};