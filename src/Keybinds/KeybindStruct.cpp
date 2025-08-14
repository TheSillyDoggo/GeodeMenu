#include "KeybindStruct.hpp"

bool KeybindStruct::isEligable()
{
    if (LevelEditorLayer::get())
        return allowInEditor;

    if (PlayLayer::get())
        return allowInGameplay;

    return allowInMenus;
}

bool KeybindStruct::canBeActivated(KeyState state)
{
    if (!isEligable())
        return false;

    if (config.shiftHeld != state.shiftHeld)
        return false;
    
    if (config.ctrlHeld != state.ctrlHeld)
        return false;
        
    if (config.altHeld != state.altHeld)
        return false;

    if (config.cmdHeld != state.cmdHeld)
        return false;

    if (config.code != state.code)
        return false;

    return true;
}