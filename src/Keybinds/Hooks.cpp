#include "Hooks.hpp"
#include <Geode/utils/Keyboard.hpp>

using namespace geode::prelude;

$execute
{
    KeyboardInputEvent()
    .listen(+[](const geode::KeyboardInputData& event)
    {
        KeyState struc;
        struc.shiftHeld = event.modifiers & KeyboardInputData::Mods_Shift;
        struc.ctrlHeld = event.modifiers & KeyboardInputData::Mods_Control;
        // struc.cmdHeld = event.modifiers & KeyboardInputData::Mods_Control;
        struc.altHeld = event.modifiers & KeyboardInputData::Mods_Alt;
        struc.code = event.key;
        struc.isDown = event.action == KeyboardInputData::Action::Press;
        struc.isRepeat = event.action == KeyboardInputData::Action::Repeat;

        if (KeybindManager::get()->processMSG(struc))
            return ListenerResult::Stop;

        return ListenerResult::Propagate;
    }).leak();

    /*MouseInputEvent()
    .listen(+[](const geode::MouseInputData& event)
    {
        return ListenerResult::Propagate;
    })
    .leak();*/
}

/*#ifndef GEODE_IS_IOS

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

#endif*/