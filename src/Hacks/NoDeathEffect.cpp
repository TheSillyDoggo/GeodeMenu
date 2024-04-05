#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

//#ifndef GEODE_IS_ANDROID32
class $modify (PlayLayer)
{
    void delayedResetLevel()
    {
        if (!Client::GetModuleEnabled("instant-restart"))
            PlayLayer::delayedResetLevel();
    }
};
//#endif

class $modify (PlayerObject)
{
    static void onModify(auto& self) {
        self.setHookPriority("PlayerObject::playDeathEffect", -6969);
    }

    void playDeathEffect()
    {
        m_isDead = true;
        
        if (Client::GetModuleEnabled("instant-restart") && PlayLayer::get())
            PlayLayer::get()->resetLevel();

        if (!Client::GetModuleEnabled("no-death"))
            PlayerObject::playDeathEffect();
    }
};