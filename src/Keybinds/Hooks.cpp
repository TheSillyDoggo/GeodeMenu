#include "Hooks.hpp"

bool KeybindDispatcher::dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat)
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

    return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
}