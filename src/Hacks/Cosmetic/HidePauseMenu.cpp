#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/PauseLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (PauseLayer)
{
    virtual void customSetup()
    {
        PauseLayer::customSetup();
        
        this->setVisible(false);
    }

    QOLMOD_MOD_HOOK("hide-pause-menu", "PauseLayer::customSetup")
};

$execute
{
    Loader::get()->queueInMainThread([] {
        Client::GetModule("hide-pause-menu")->onToggle = [](bool enabled)
        {
            if (auto p = CCScene::get()->getChildByType<PauseLayer>(0))
            {
                p->setVisible(!enabled);
            }
        };
    });
}