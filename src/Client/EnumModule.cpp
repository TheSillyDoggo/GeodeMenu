#include "EnumModule.hpp"
#include "EnumModuleNode.hpp"

using namespace geode::prelude;

void EnumModule::save()
{
    if (!shouldSave())
        return;
    
    Mod::get()->setSavedValue<int>(fmt::format("{}_value", getID()), getValue());
}

void EnumModule::load()
{
    setValue(Mod::get()->getSavedValue<int>(fmt::format("{}_value", getID()), getDefaultValue()));
}

void EnumModule::setValue(int value)
{
    this->value = value;

    save();
}

int EnumModule::getValue()
{
    return value;
}

int EnumModule::getDefaultValue()
{
    return defaultValue;
}

std::map<int, std::string> EnumModule::getDisplayNames()
{
    return listedValues;
}

ModuleNode* EnumModule::getNode()
{
    return EnumModuleNode::create(this);
}