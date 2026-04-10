#pragma once

#include "../../Client/Module.hpp"
#include <PopupBase.hpp>

using namespace geode::prelude;

class ShowButtonInEditor : public Module
{
    public:
        MODULE_SETUP(ShowButtonInEditor)
        {
            setID("show-button-in-editor");
            setCategory("Config/Button");
            setDefaultEnabled(true);
        }
};

class ShowButtonInEndScreen : public Module
{
    public:
        MODULE_SETUP(ShowButtonInEndScreen)
        {
            setID("show-button-in-endscreen");
            setCategory("Config/Button");
            setDefaultEnabled(true);
        }
};

class ShowButtonInGame : public Module
{
    public:
        MODULE_SETUP(ShowButtonInGame)
        {
            setID("show-button-in-game");
            setCategory("Config/Button");
            setDefaultEnabled(false);
        }
};

SUBMIT_HACK(ShowButtonInEditor);
SUBMIT_HACK(ShowButtonInEndScreen);
SUBMIT_HACK(ShowButtonInGame);