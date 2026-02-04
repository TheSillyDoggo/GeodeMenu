#include "Hooks.hpp"

#ifndef GEODE_IS_IOS

bool KeybindDispatcher::dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat, double unk)
{
    KeyState struc;
    struc.shiftHeld = getShiftKeyPressed();
    struc.ctrlHeld = getControlKeyPressed();
    struc.cmdHeld = getCommandKeyPressed();
    struc.altHeld = getAltKeyPressed();
    struc.code = key;
    struc.isDown = down;
    struc.isRepeat = repeat;

    if (KeybindManager::get()->processMSG(struc))
        return true;

    return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat, unk);
}

#endif