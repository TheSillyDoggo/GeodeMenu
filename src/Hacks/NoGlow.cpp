#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(GameObject) {
    
    void setVisible(bool v) {
        if (Client::GetModuleEnabled("no-glow"))
            m_hasNoGlow = true;

        //if (Client::GetModuleEnabled("dont-fade"))
            //m_isDontFade = true;

        //if (Client::GetModuleEnabled("dont-enter"))
            //m_isDontEnter = true;

        if (Client::GetModuleEnabled("force-obj-vis"))
            GameObject::setVisible(true);

        GameObject::setVisible(v);
    }
};