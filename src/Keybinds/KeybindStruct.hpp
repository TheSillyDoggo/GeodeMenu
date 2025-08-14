#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct KeyState
{
    bool shiftHeld = false;
    bool ctrlHeld = false;
    bool altHeld = false;
    bool cmdHeld = false;

    enumKeyCodes code = enumKeyCodes::KEY_None;
    bool isDown = false;
    bool isRepeat = false;
};

struct KeybindStruct
{
    std::string name;
    std::string id;
    KeyState config;
    bool allowInGameplay = true;
    bool allowInEditor = true;
    bool allowInMenus = true;

    bool isEligable();
    bool canBeActivated(KeyState state);

    virtual void onActivate(KeyState state) {}
};