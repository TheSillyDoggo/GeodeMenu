#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCActionManager.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    static void onModify(auto& self) {
        self.setHookPriority("PlayerObject::playDeathEffect", -6969);
    }

    void playDeathEffect()
    {
        m_isDead = true;
        
        if (Client::GetModuleEnabled("instant-restart") && PlayLayer::get())
        {
            PlayLayer::get()->resetLevel();
            PlayLayer::get()->stopActionByTag(16);
            this->setVisible(true);
        }

        if (!Client::GetModuleEnabled("no-death"))
            PlayerObject::playDeathEffect();
    }
};