#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/PlayLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify(PlayLayer)
{
    void addObject(GameObject* obj)
    {
        obj->m_hasNoGlow = true;

        PlayLayer::addObject(obj);
    }

    QOLMOD_MOD_HOOK("no-glow", "PlayLayer::addObject")
};