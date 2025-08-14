#include "KeybindManager.hpp"
#include "ModuleKeybindStruct.hpp"

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

    for (auto& bind : structs)
    {
        if (bind->canBeActivated(state))
        {
            bind->onActivate(state);

            capture = true;
        }
    }

    return capture;
}

$execute
{
    // auto str = new ModuleKeybindStruct();
    // str->modID = "show-hitboxes";
    // str->type = KeybindType::Hold;
    // str->config.code = enumKeyCodes::KEY_G;

    // KeybindManager::get()->addStruct(str);
}