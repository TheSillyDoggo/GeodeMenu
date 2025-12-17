#pragma once

#include <string>
#include <Geode/Geode.hpp>
#include "KeyState.hpp"

enum class KeybindType
{
    Toggle,
    Hold
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

    std::string configToString();

    bool isActivated(KeyState state);
    bool isValid();

    matjson::Value save();
    void load(matjson::Value value);
};