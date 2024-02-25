#ifndef GEODE_IS_ANDROID32

#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(GameObject) {
    
    TodoReturn commonSetup() 
    {
        GameObject::commonSetup();

        if (PlayLayer::get())
        {
            if (Client::GetModuleEnabled("no-glow"))
                m_hasNoGlow = true;

            m_isDontFade = true;
            m_isDontEnter = true;

            if (m_objectID == 45 || m_objectID == 46)
            {
                log::info("mirror");

                m_isNoTouch = true;
            }
        }
    }
};

#endif