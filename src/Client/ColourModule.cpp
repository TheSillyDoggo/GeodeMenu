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
    Mod::get()->setSavedValue<bool>(fmt::format("{}_smoothgradient", getID()), userConf.smoothGradient);

    matjson::Value save = matjson::Value::array();
    for (auto grad : userConf.gradientLocations)
    {
        matjson::Value value;

        value["location"] = grad.percentageLocation;
        value["colour"] = grad.colour;

        save.push(value);
    }

    Mod::get()->setSavedValue<matjson::Value>(fmt::format("{}_gradientlocations", getID()), save);
}

void ColourModule::load()
{
    ColourConfig conf;

    conf.customColour = Mod::get()->getSavedValue<ccColor3B>(fmt::format("{}_colour", getID()), defaultConf.customColour);
    conf.opacity = Mod::get()->getSavedValue<float>(fmt::format("{}_opacity", getID()), defaultConf.opacity);
    conf.chromaSpeed = Mod::get()->getSavedValue<float>(fmt::format("{}_chromaspeed", getID()), defaultConf.chromaSpeed);
    conf.type = (ColourConfigType)Mod::get()->getSavedValue<int>(fmt::format("{}_type", getID()), (int)defaultConf.type);
    conf.smoothGradient = Mod::get()->getSavedValue<bool>(fmt::format("{}_smoothgradient", getID()), defaultConf.smoothGradient);

    matjson::Value save = Mod::get()->getSavedValue<matjson::Value>(fmt::format("{}_gradientlocations", getID()), "{}");
    if (save.isArray())
    {
        conf.gradientLocations.clear();

        for (auto grad : save.asArray().unwrap())
        {
            conf.gradientLocations.push_back(
            {
                ccc3(
                    grad["colour"]["r"].asInt().unwrapOr(255),
                    grad["colour"]["g"].asInt().unwrapOr(255),
                    grad["colour"]["b"].asInt().unwrapOr(255)
                ),
                (float)grad["location"].asDouble().unwrapOr(0.5f)
            });
        }
    }
    else
    {
        conf.gradientLocations = defaultConf.gradientLocations;
    }

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