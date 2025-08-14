#pragma once

#include "KeybindManager.hpp"
#include <Geode/modify/CCKeyboardDispatcher.hpp>

class $modify (KeybindDispatcher, CCKeyboardDispatcher)
{
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat);
};