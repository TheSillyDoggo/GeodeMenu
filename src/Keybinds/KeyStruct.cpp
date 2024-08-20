#include "KeyStruct.hpp"

using namespace geode::prelude;

KeyStruct KeyStruct::loadFromModule(std::string id)
{
    KeyStruct key;

    key.key = as<enumKeyCodes>(Mod::get()->getSavedValue<int>(fmt::format("{}_bind-key", id), -1));
    key.shift = Mod::get()->getSavedValue<bool>(fmt::format("{}_bind-shift", id), key.shift);
    key.alt = Mod::get()->getSavedValue<bool>(fmt::format("{}_bind-alt", id), key.alt);
    key.canRepeat = Mod::get()->getSavedValue<bool>(fmt::format("{}_bind-can-repeat", id), key.canRepeat);
    key.command = Mod::get()->getSavedValue<bool>(fmt::format("{}_bind-cmd", id), key.command);
    key.control = Mod::get()->getSavedValue<bool>(fmt::format("{}_bind-ctrl", id), key.control);

    return key;
}

void KeyStruct::saveToModule(std::string id)
{
    Mod::get()->setSavedValue<int >(fmt::format("{}_bind-key"       , id), as<int>(key));
    Mod::get()->setSavedValue<bool>(fmt::format("{}_bind-shift"     , id), shift);
    Mod::get()->setSavedValue<bool>(fmt::format("{}_bind-alt"       , id), alt);
    Mod::get()->setSavedValue<bool>(fmt::format("{}_bind-can-repeat", id), canRepeat);
    Mod::get()->setSavedValue<bool>(fmt::format("{}_bind-cmd"       , id), command);
    Mod::get()->setSavedValue<bool>(fmt::format("{}_bind-ctrl"      , id), control);
}

std::string KeyStruct::toString()
{
    if (key == enumKeyCodes::KEY_Unknown)
        return "";

    std::stringstream ss;

    if (control)
        ss << "Ctrl + ";

    if (alt)
        ss << "Alt + ";

    if (shift)
        ss << "Shift + ";

    if (command)
        ss << "Cmd + ";

    if (auto k = CCKeyboardDispatcher::get()->keyToString(key))
        ss << k;

    return ss.str();
}