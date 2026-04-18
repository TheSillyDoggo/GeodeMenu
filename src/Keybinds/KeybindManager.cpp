#include "KeybindManager.hpp"
#include "FuncKeybindStruct.hpp"
#include "../GUI/Modules/MenuKeybind.hpp"
#include "../GUI/AndroidUI.hpp"
#include "../Hacks/Speedhack/Speedhack.hpp"
#include "../GUI/Categories/SpeedhackNode.hpp"
#include "../Utils/Num.hpp"
#include <NotificationManager.hpp>
#include <LocalisationManager.hpp>
#include <EditKeyConfigUI.hpp>

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
    if (EditKeyConfigUI::get())
        return false;

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

            bool enabled = Speedhack::get()->getEnabled();

            switch (preset.keyConfig.type)
            {
                case KeybindType::Toggle:
                    if (!(state.isDown || state.isRepeat))
                        continue;
                    
                    enabled = prev == str ? !Speedhack::get()->getEnabled() : true;
                    break;

                case KeybindType::Hold:
                    if (state.isDown)
                        enabled = true;

                    if (!state.isDown && !state.isRepeat)
                        enabled = false;
                    break;

                case KeybindType::HoldInverted:
                    if (state.isDown)
                        enabled = false;

                    if (!state.isDown && !state.isRepeat)
                        enabled = true;
                    break;
            }

            if (enabled == Speedhack::get()->getEnabled() && str == prev)
                continue;

            Speedhack::get()->setText(str);
            Speedhack::get()->setEnabled(enabled);

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