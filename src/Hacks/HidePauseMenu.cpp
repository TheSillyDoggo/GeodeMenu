#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../Client/Client.h"

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
        Module::get("hide-pause-menu")->onToggle = [](bool enabled)
        {
            if (auto p = CCScene::get()->getChildByType<PauseLayer>(0))
            {
                p->setVisible(!enabled);
            }
        };
    });
}