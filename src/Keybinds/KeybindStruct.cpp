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

    if (config.shiftRequired != state.shiftHeld)
        return false;
    
    if (config.ctrlRequired != state.ctrlHeld)
        return false;
        
    if (config.altRequired != state.altHeld)
        return false;

    if (config.cmdRequired != state.cmdHeld)
        return false;

    if (config.code != state.code)
        return false;

    return true;
}