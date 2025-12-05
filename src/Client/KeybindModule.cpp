#include "KeybindModule.hpp"
#include "KeybindModuleNode.hpp"

void KeybindModule::save()
{
    if (!shouldSave())
        return;
    
    Mod::get()->setSavedValue<int>(fmt::format("{}_keycode", getID()), getKeyCode());
}

void KeybindModule::load()
{
    setKeyCode(Mod::get()->getSavedValue<int>(fmt::format("{}_keycode", getID()), getDefaultKeyCode()));
}

void KeybindModule::setDefaultKeyCode(int defaultCode)
{
    this->defaultKeyCode = defaultCode;
}

void KeybindModule::setKeyCode(int code)
{
    this->keyCode = code;

    save();
}

int KeybindModule::getKeyCode()
{
    return this->keyCode;
}

int KeybindModule::getDefaultKeyCode()
{
    return this->defaultKeyCode;
}

bool KeybindModule::isBindRequired()
{
    return this->mustHaveBind;
}

void KeybindModule::setBindRequired(bool req)
{
    this->mustHaveBind = req;
}

ModuleNode* KeybindModule::getNode()
{
    return KeybindModuleNode::create(this);
}