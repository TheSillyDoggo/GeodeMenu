#pragma once

#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"

using namespace geode::prelude;

class OpenSettings : public ButtonModule
{
    public:
        MODULE_SETUP(OpenSettings)
        {
            setID("shortcut/open-settings");
            setCategory("Universal");

            setPriority(sortPriority + 2);
        }

        virtual void onClick()
        {
            auto ui = OptionsLayer::create();
            ui->showLayer(false);
            ui->setZOrder(CCScene::get()->getHighestChildZ() + 1);
        }
};

SUBMIT_HACK(OpenSettings);