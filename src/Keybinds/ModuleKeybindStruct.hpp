#pragma once

#include "KeybindStruct.hpp"

enum class KeybindType
{
    Toggle,
    Hold
};

struct ModuleKeybindStruct : public KeybindStruct
{
    KeybindType type;
    bool invertHold;
    std::string modID;

    virtual void onActivate(KeyState state);
};