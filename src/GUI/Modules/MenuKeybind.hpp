#pragma once

#include "../../Client/KeybindModule.hpp"

using namespace geode::prelude;

class MenuKeybind : public KeybindModule
{
    public:
        MODULE_SETUP(MenuKeybind)
        {
            setName("Menu Keybind:");
            setID("internal/open-menu");
            setDescription("");
            setCategory("Config/Menu");

            setDefaultKeyCode(enumKeyCodes::KEY_Tab); // Tab
            setBindRequired(true);
        }
};

SUBMIT_HACK(MenuKeybind);