#include "FloatSliderModule.hpp"
#include "FloatSliderModuleNode.hpp"

using namespace geode::prelude;

void FloatSliderModule::save()
{
    if (!shouldSave())
        return;
    
    Mod::get()->setSavedValue<float>(fmt::format("{}_value", getID()), getValue());
}

void FloatSliderModule::load()
{
    setValue(Mod::get()->getSavedValue<float>(fmt::format("{}_value", getID()), getDefaultValue()));
}

void FloatSliderModule::setRange(float min, float max)
{
    this->minRange = min;
    this->maxRange = max;
}

void FloatSliderModule::setDefaultValue(float def)
{
    this->defaultValue = def;
}

void FloatSliderModule::setSnapValues(std::vector<float> snaps)
{
    this->snaps = snaps;
}

void FloatSliderModule::setValue(float value)
{
    this->value = value;

    save();
}

float FloatSliderModule::getValue()
{
    return value;
}

float FloatSliderModule::getDefaultValue()
{
    return defaultValue;
}

ModuleNode* FloatSliderModule::getNode()
{
    return FloatSliderModuleNode::create(this);
}