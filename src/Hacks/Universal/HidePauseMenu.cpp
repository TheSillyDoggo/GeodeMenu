#include "../../Client/Module.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class HidePauseMenu : public Module
{
    public:
        MODULE_SETUP(HidePauseMenu)
        {
            setName("Hide Pause Menu");
            setID("hide-pause-menu");
            setCategory("Universal");
            setDescription("Hides the pause menu :3");
        }

        void onToggle()
        {
            if (auto pause = CCScene::get()->getChildByType<PauseLayer>(0))
            {
                pause->setVisible(!getRealEnabled());
            }
        }
};

SUBMIT_HACK(HidePauseMenu)

class $modify (PauseLayer)
{
    virtual void customSetup()
    {
        PauseLayer::customSetup();

        this->setVisible(!HidePauseMenu::get()->getRealEnabled());
    }
};