#include "RotationModule.hpp"
#include "RotationModuleNode.hpp"

using namespace geode::prelude;

void RotationModule::save()
{
    if (!shouldSave())
        return;
    
    Mod::get()->setSavedValue<float>(fmt::format("{}_rotation", getID()), getRotationDegrees());
}

void RotationModule::load()
{
    setRotation(Mod::get()->getSavedValue<float>(fmt::format("{}_rotation", getID()), getDefaultValue()));
}

void RotationModule::setDefaultValue(float v)
{
    this->defaultValue = v;
}

void RotationModule::setRotation(float value)
{
    this->rotation = value;

    save();
}

float RotationModule::getRotationDegrees()
{
    return rotation;
}

CCPoint RotationModule::getRotationVector()
{
    float angleRadians = CC_DEGREES_TO_RADIANS(rotation);

    float dx = std::cos(angleRadians);
    float dy = std::sin(angleRadians);

    return ccp(dx, dy);
}

float RotationModule::getDefaultValue()
{
    return defaultValue;
}

ModuleNode* RotationModule::getNode()
{
    return RotationModuleNode::create(this);
}