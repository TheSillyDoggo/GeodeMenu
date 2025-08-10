#include "InputModule.hpp"
#include "InputModuleNode.hpp"

using namespace geode::prelude;

void InputModule::save()
{
    if (!shouldSave())
        return;
    
    Mod::get()->setSavedValue<std::string>(fmt::format("{}_value", getID()), getString());
}

void InputModule::load()
{
    setString(Mod::get()->getSavedValue<std::string>(fmt::format("{}_value", getID()), getDefaultString()));
}

void InputModule::setDefaultString(std::string str)
{
    this->defaultText = str;
}

void InputModule::setPlaceholderString(std::string str)
{
    this->placeholder = str;
}

void InputModule::setString(std::string str)
{
    this->text = str;
    this->textFloat = 1.0f;

    auto res = utils::numFromString<float>(str);
    this->textFloat = res.unwrapOr(1.0f);

    onToggle();
    save();
}

std::string InputModule::getString()
{
    return text;
}

float InputModule::getStringFloat()
{
    return textFloat;
}

int InputModule::getStringInt()
{
    return as<int>(textFloat);
}

std::string InputModule::getDefaultString()
{
    return defaultText;
}

std::string InputModule::getPlaceholderString()
{
    return placeholder;
}

bool InputModule::appendColon()
{
    return true;
}

void InputModule::setStringFilter(std::string str)
{
    this->filter = str;
}

void InputModule::setStringFilterCommon(geode::CommonFilter filter)
{
    this->filter = getCommonFilterAllowedChars(filter);
}

void InputModule::setMaxCharCount(int count)
{
    this->charLimit = count;
}

void InputModule::setDisplayFilter(std::string str)
{
    this->displayFilter = str;
}

ModuleNode* InputModule::getNode()
{
    return InputModuleNode::create(this);
}

int InputModule::getMaxCharCount()
{
    return charLimit;
}

std::string InputModule::getStringFilter()
{
    return filter;
}

std::string InputModule::getDisplayFilter()
{
    return displayFilter;
}