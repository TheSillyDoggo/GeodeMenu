#pragma once

#include <string>
#include <Geode/Geode.hpp>
#include "KeyState.hpp"

enum class KeybindType
{
    Toggle,
    Hold,
    HoldInverted,
};

enum class KeyModifier
{
    Shift,
    Control,
    Alt,
    Command
};

using Keycode = cocos2d::enumKeyCodes;

struct KeyConfigStruct
{
    std::vector<KeyModifier> modifiers = {};
    Keycode code = Keycode::KEY_Unknown;
    KeybindType type = KeybindType::Toggle;

    std::string configToString();

    bool isActivated(KeyState state);
    bool isValid();

    matjson::Value save();
    void load(matjson::Value value);
};