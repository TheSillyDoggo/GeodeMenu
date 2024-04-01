#ifndef GEODE_IS_ANDROID32

#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* noGlow = nullptr;

class $modify(GameObject) {
    
    void commonSetup() 
    {
        GameObject::commonSetup();

        if (!noGlow)
            noGlow = Client::GetModule("no-glow");

        if (PlayLayer::get())
        {
            if (noGlow->enabled)
                m_hasNoGlow = true;
        }
    }
};
#endif