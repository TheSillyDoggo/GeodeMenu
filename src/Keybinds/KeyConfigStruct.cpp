#include "KeyConfigStruct.hpp"

std::string KeyConfigStruct::configToString()
{
    std::string mod = "";
    
    for (auto modifier : modifiers)
    {
        switch (modifier)
        {
            case KeyModifier::Alt:
                mod += "Alt + ";
                break;

            case KeyModifier::Control:
                mod += "Ctrl + ";
                break;

            case KeyModifier::Command:
                mod += "Cmd + ";
                break;

            case KeyModifier::Shift:
                mod += "Shift + ";
                break;
        }
    }

    auto kts = cocos2d::CCKeyboardDispatcher::get()->keyToString(code);
    std::string key = kts ? kts : "Unknown";

    if (code == Keycode::KEY_C)
        key = "C"; // for some reason it says "V" instead of C

    return fmt::format("{}{}", mod, key);
}

bool KeyConfigStruct::isActivated(KeyState state)
{
    if (code == (int)Keycode::KEY_Unknown || code == (int)Keycode::KEY_None || code == (int)Keycode::KEY_LeftWindowsKey)
        return false;

    for (auto mod : modifiers)
    {
        switch (mod)
        {
            case KeyModifier::Control:
                if (!state.ctrlHeld)
                    return false;

                break;
            
            case KeyModifier::Alt:
                if (!state.altHeld)
                    return false;

                break;

            case KeyModifier::Shift:
                if (!state.shiftHeld)
                    return false;

                break;

            case KeyModifier::Command:
                if (!state.cmdHeld)
                    return false;

                break;
        }
    }
    

    if (state.isDown)
    {
        if (state.code == (int)code)
            return true;
    }

    return false;
}

matjson::Value KeyConfigStruct::save()
{
    matjson::Value obj;

    matjson::Value modsArr = obj.array();
    
    for (auto modifier : modifiers)
    {
        modsArr.asArray().unwrap().push_back((int)modifier);
    }

    obj["modifiers"] = modsArr;
    obj["code"] = (int)code;

    return obj;
}

void KeyConfigStruct::load(matjson::Value value)
{
    code = (Keycode)value["code"].asInt().unwrapOr(-1);

    if (value.contains("modifiers") && value["modifiers"].isArray())
    {
        for (auto modifier : value["modifiers"].asArray().unwrap())
        {
            this->modifiers.push_back((KeyModifier)modifier.asInt().unwrapOr(0));
        }
    }
}