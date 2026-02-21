#include "KeybindManager.hpp"
#include "FuncKeybindStruct.hpp"
#include "../GUI/Modules/MenuKeybind.hpp"
#include "../GUI/AndroidUI.hpp"
#include "../Hacks/Speedhack/Speedhack.hpp"
#include "../GUI/Categories/SpeedhackNode.hpp"
#include "../Utils/Num.hpp"
#include <NotificationManager.hpp>
#include <LocalisationManager.hpp>

KeybindManager* KeybindManager::get()
{
    static KeybindManager* instance = nullptr;

    if (!instance)
        instance = new KeybindManager();

    return instance;
}

void KeybindManager::addStruct(KeybindStruct* str)
{
    structs.push_back(str);
}

KeybindStruct* KeybindManager::getStruct(std::string id)
{
    for (auto& str : structs)
    {
        if (str->id == id)
            return str;
    }

    return nullptr;
}

bool KeybindManager::processMSG(KeyState state)
{
    bool capture = false;

    for (auto mod : Module::getAll())
    {
        if (mod->getKeybind().isActivated(state))
        {
            mod->onKeybindActivated(state);
        }
    }

    for (auto preset : Speedhack::get()->getPresets())
    {
        if (preset.keyConfig.isActivated(state))
        {
            float val = preset.value;
            auto str = floatToStringMin2DP(val);

            auto prev = Speedhack::get()->getText();

            Speedhack::get()->setText(str);
            Speedhack::get()->setEnabled(prev == str ? !Speedhack::get()->getEnabled() : true);

            if (SpeedhackNode::get())
                SpeedhackNode::get()->updateUI();

            if (prev != str)
            {
                NotificationManager::get()->notifyToast(utils::string::replace(LocalisationManager::get()->getLocalisedString("ui/notification-speedhack-set"), "%s", str));
            }
            else
            {
                NotificationManager::get()->notifyToast(SpeedhackEnabled::get()->getNotificationString());
            }

            capture = true;
        }
    }

    for (auto& bind : structs)
    {
        if (bind->canBeActivated(state))
        {
            if (bind->config.activateOnDown == state.isDown && bind->config.activateOnRepeat == state.isRepeat)
            {
                bind->onActivate(state);

                capture = true;
            }
        }
    }

    return capture;
}

#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

$on_mod(Loaded)
{
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
};