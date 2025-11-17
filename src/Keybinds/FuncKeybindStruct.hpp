#pragma once

#include "KeybindStruct.hpp"

struct FuncKeybindStruct : public KeybindStruct
{
    std::function<void(KeyState state)> func = nullptr;

    virtual void onActivate(KeyState state);
};