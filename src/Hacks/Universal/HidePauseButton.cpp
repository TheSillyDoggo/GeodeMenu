#include "../../Client/Module.hpp"
#include <Geode/modify/UILayer.hpp>

using namespace geode::prelude;

class HidePauseButton : public Module
{
    public:
        MODULE_SETUP(HidePauseButton)
        {
            setName("Hide Pause Button");
            setID("hide-pause-button");
            setCategory("Universal");
            setDescription("Hides the pause button in game, requires reopening level to apply");
        }
};

SUBMIT_HACK(HidePauseButton)

class $modify (UILayer)
{
    bool init(GJBaseGameLayer* p0)
    {
        if (!UILayer::init(p0))
            return false;

        if (HidePauseButton::get()->getRealEnabled())
        {
            if (auto menu = this->getChildByType<CCMenu>(0); auto btn = menu->getChildByType<CCMenuItemSpriteExtra>(0))
            {
                btn->getNormalImage()->setVisible(false);
            }
        }

        return true;
    }
};