#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"

using namespace geode::prelude;

class OpenSettings : public ButtonModule
{
    public:
        MODULE_SETUP(OpenSettings)
        {
            setID("shortcut/open-settings");
            setCategory("Shortcuts");

            setPriority(sortPriority + 2);
        }

        virtual void onClick()
        {
            auto ui = OptionsLayer::create();
            CCScene::get()->addChild(ui);
            ui->showLayer(false);
            ui->setZOrder(CCScene::get()->getHighestChildZ() + 1);
        }
};

SUBMIT_HACK(OpenSettings);