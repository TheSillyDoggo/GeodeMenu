#include "Hooks.hpp"
#include <Geode/utils/Keyboard.hpp>
#include <Geode/modify/CCIMEDispatcher.hpp>
#include <BetterInputNode.hpp>
#include "Casts.hpp"

using namespace geode::prelude;

$execute
{
    KeyboardInputEvent()
    .listen(+[](const geode::KeyboardInputData& event)
    {
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
}