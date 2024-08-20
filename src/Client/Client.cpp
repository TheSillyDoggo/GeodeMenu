#include "Client.h"

Client* Client::get()
{
    return instance;
}

bool Client::handleKeybinds(enumKeyCodes key, bool isDown, bool isRepeatedKey)
{
    if (!isDown || key == enumKeyCodes::KEY_Unknown)
        return false;

    bool shouldPropogate = true;

    for (auto window : windows)
    {
        for (auto module : window->modules)
        {
            if (isRepeatedKey ? module->keybind.canRepeat : true)
            {
                bool shouldSend = true;

                if (module->keybind.key == enumKeyCodes::KEY_Unknown)
                    shouldSend = false;

                if (module->keybind.shift && !CCKeyboardDispatcher::get()->getShiftKeyPressed())
                    shouldSend = false;

                if (module->keybind.control && !CCKeyboardDispatcher::get()->getControlKeyPressed())
                    shouldSend = false;

                if (module->keybind.alt && !CCKeyboardDispatcher::get()->getAltKeyPressed())
                    shouldSend = false;

                if (module->keybind.command && !CCKeyboardDispatcher::get()->getCommandKeyPressed())
                    shouldSend = false;

                if (shouldSend)
                {
                    module->onToggleAndroid(nullptr);

                    log::info("toggled: {}", module->id);

                    if (!shouldPropogate)
                        return true;
                }
            }
        }
    }

    return false;
}