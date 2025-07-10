#include "Hooks.hpp"
#include "AndroidUI.hpp"

bool GUIKeyboardDispatcher::dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat)
{
    if (!CCScene::get())
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);

    if (down && key == enumKeyCodes::KEY_Tab)
    {
        if (AndroidUI::get())
            AndroidUI::get()->close();
        else
            AndroidUI::addToScene();

        return true;
    }

    return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
}