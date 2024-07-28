#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(PlayLayer)
{
    void addObject(GameObject* obj)
    {
        obj->m_isDontFade = true;

        PlayLayer::addObject(obj);
    }

    QOLMOD_MOD_HOOK("dont-fade", "PlayLayer::addObject")
};

class $modify(PlayLayer)
{
    void addObject(GameObject* obj)
    {
        obj->m_isDontEnter = true;

        PlayLayer::addObject(obj);
    }

    QOLMOD_MOD_HOOK("dont-enter", "PlayLayer::addObject")
};