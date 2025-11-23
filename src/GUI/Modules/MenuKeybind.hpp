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
            setCategory("Config");

            setDefaultKeyCode(enumKeyCodes::KEY_Tab); // Tab
        }
};

// SUBMIT_HACK(MenuKeybind);