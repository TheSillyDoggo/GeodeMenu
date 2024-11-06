#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCActionManager.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    static void onModify(auto& self) {
        (void)self.setHookPriority("PlayerObject::playDeathEffect", -6969);
    }

    void playDeathEffect()
    {
        m_isDead = true;
        
        if (!Client::GetModuleEnabled("no-death"))
            PlayerObject::playDeathEffect();
    }
};

class $modify (PlayLayer)
{
    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        PlayLayer::destroyPlayer(p0, p1);

        if (!p0)
            return;

        if (Client::GetModuleEnabled("instant-restart") || Client::GetModuleEnabled("custom-respawn-time"))
        {
            if (auto action = getActionByTag(0x10))
            {
                this->stopActionByTag(0x10);

                if (Client::GetModuleEnabled("instant-restart"))
                {
                    this->resetLevel();
                    p0->setVisible(true);
                    return;
                }
                else
                {
                    auto act = CCSequence::create(CCDelayTime::create(as<InputModule*>(Client::GetModule("custom-respawn-time")->options[0])->getFloatValue()), CCCallFunc::create(this, callfunc_selector(PlayLayer::delayedResetLevel)), nullptr);
                    act->setTag(0x10);

                    this->runAction(act);
                }
            }
        }
    }
};