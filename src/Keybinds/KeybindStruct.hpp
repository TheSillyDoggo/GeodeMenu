#pragma once

#include <Geode/Geode.hpp>
#include "../Client/KeybindModule.hpp"

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

struct KeyConfigState
{
    bool shiftRequired = false;
    bool ctrlRequired = false;
    bool altRequired = false;
    bool cmdRequired = false;

    enumKeyCodes code = enumKeyCodes::KEY_None;
    KeybindModule* mod = nullptr;
    bool activateOnDown = false;
    bool activateOnRepeat = false;
    bool activateOnRelease = false;
};

struct KeybindStruct
{
    std::string name;
    std::string id;
    KeyConfigState config;
    bool allowInGameplay = true;
    bool allowInEditor = true;
    bool allowInMenus = true;
    bool sendNotification = false;

    bool isEligable();
    bool canBeActivated(KeyState state);

    virtual void onActivate(KeyState state) {}
};