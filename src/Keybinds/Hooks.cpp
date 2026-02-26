#include "Hooks.hpp"
#include <Geode/utils/Keyboard.hpp>
#include <Geode/modify/CCIMEDispatcher.hpp>
#include <BetterInputNode.hpp>
#include "Casts.hpp"
#include <AndroidUI.hpp>
#include <Modules/SearchBox.hpp>

using namespace geode::prelude;

$execute
{
    KeyboardInputEvent()
    .listen(+[](const geode::KeyboardInputData& event)
    {
        if (auto ui = AndroidUI::get())
        {
            if (SearchOnKeyPress::get()->getRealEnabled() && event.action == KeyboardInputData::Action::Press)
            {
                ui->switchTabTemp("Search");
            }
        }

        if (CCIMEDispatcher::sharedDispatcher()->hasDelegate())
        {
            if (auto inp = BetterInputNode::getSelected())
            {
                inp->handleKeypress(event.key, event.action);
            }

            return ListenerResult::Propagate;
        }

        KeyState struc;
        struc.shiftHeld = event.modifiers & KeyboardModifier::Shift;
        struc.ctrlHeld = event.modifiers & KeyboardModifier::Control;
        struc.cmdHeld = event.modifiers & KeyboardModifier::Super;
        struc.altHeld = event.modifiers & KeyboardModifier::Alt;
        struc.code = event.key;
        struc.isDown = event.action == KeyboardInputData::Action::Press;
        struc.isRepeat = event.action == KeyboardInputData::Action::Repeat;

        if (KeybindManager::get()->processMSG(struc))
            return ListenerResult::Stop;

        return ListenerResult::Propagate;
    }).leak();

    listenForKeybindSettingPresses("open-menu-keybind", [](Keybind const& keybind, bool down, bool repeat, double timestamp)
    {
        if (down && !repeat)
        {
            if (AndroidUI::get())
            {
                // close one menu so that option displays are gone before the main popup
                CCKeyboardDispatcher::get()->dispatchKeyboardMSG(enumKeyCodes::KEY_Escape, true, false, 0);
            }            
            else
            {
                AndroidUI::addToScene();
            }
        }
    });
}