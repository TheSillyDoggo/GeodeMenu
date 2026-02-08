#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"

using namespace geode::prelude;

class OpenGraphicsSettings : public ButtonModule
{
    public:
        MODULE_SETUP(OpenGraphicsSettings)
        {
            setID("shortcut/open-graphics-settings");
            setCategory("Shortcuts");

            setPriority(sortPriority + 2);
        }

        virtual void onClick()
        {
            auto ui = VideoOptionsLayer::create();
            ui->show();
            ui->setZOrder(CCScene::get()->getHighestChildZ() + 1);
        }
};

SUBMIT_HACK(OpenGraphicsSettings);