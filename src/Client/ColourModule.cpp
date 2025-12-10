#include "ColourModule.hpp"
#include "../Utils/ColourUtils.hpp"
#include "ColourModuleNode.hpp"

using namespace geode::prelude;

void ColourModule::save()
{
    if (!shouldSave())
        return;

    Mod::get()->setSavedValue<ccColor3B>(fmt::format("{}_colour", getID()), userConf.customColour);
    Mod::get()->setSavedValue<float>(fmt::format("{}_opacity", getID()), userConf.opacity);
    Mod::get()->setSavedValue<float>(fmt::format("{}_chromaspeed", getID()), userConf.chromaSpeed);
    Mod::get()->setSavedValue<int>(fmt::format("{}_type", getID()), (int)userConf.type);
}

void ColourModule::load()
{
    ColourConfig conf;

    conf.customColour = Mod::get()->getSavedValue<ccColor3B>(fmt::format("{}_colour", getID()), defaultConf.customColour);
    conf.opacity = Mod::get()->getSavedValue<float>(fmt::format("{}_opacity", getID()), defaultConf.opacity);
    conf.chromaSpeed = Mod::get()->getSavedValue<float>(fmt::format("{}_chromaspeed", getID()), defaultConf.chromaSpeed);
    conf.type = (ColourConfigType)Mod::get()->getSavedValue<int>(fmt::format("{}_type", getID()), (int)defaultConf.type);

    setConfig(conf);
}

void ColourModule::setDefaultConfig(ColourConfig config)
{
    this->defaultConf = config;
}

void ColourModule::setConfig(ColourConfig config)
{
    this->userConf = config;

    ColourUtils::get()->setChannelSpeed(getID(), userConf.chromaSpeed);

    save();
}

ColourConfig ColourModule::getConfig()
{
    return userConf;
}

ColourConfig ColourModule::getDefaultConfig()
{
    return defaultConf;
}

ccColor3B ColourModule::getColour()
{
    return userConf.colourForConfig(getID());
}

ModuleNode* ColourModule::getNode()
{
    return ColourModuleNode::create(this);
}