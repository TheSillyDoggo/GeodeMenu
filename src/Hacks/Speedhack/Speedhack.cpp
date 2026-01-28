#include "Speedhack.hpp"
#include <Geode/Geode.hpp>
#include <LocalisationManager.hpp>

using namespace geode::prelude;

Speedhack* Speedhack::get()
{
    static Speedhack* instance = nullptr;

    if (!instance)
    {
        instance = new Speedhack();
        instance->setup();
    }

    return instance;
}

void Speedhack::setup()
{
    setGameplayEnabled(Mod::get()->getSavedValue<bool>("speedhack-gameplay_enabled", false));
    setText(Mod::get()->getSavedValue<std::string>("speedhack-top_value", "1.0")); // defaults
    loadPresets();
}

bool Speedhack::getEnabled()
{
    return SpeedhackEnabled::get()->getRealEnabled();
}

void Speedhack::setEnabled(bool value)
{
    SpeedhackEnabled::get()->setUserEnabled(value);
}

bool Speedhack::getMusicEnabled()
{
    return SpeedhackMusic::get()->getRealEnabled();
}

void Speedhack::setMusicEnabled(bool enabled)
{
    SpeedhackMusic::get()->setUserEnabled(enabled);
}

bool Speedhack::getGameplayEnabled()
{
    return SpeedhackGameplay::get()->getRealEnabled();
}

void Speedhack::setGameplayEnabled(bool enabled)
{
    SpeedhackGameplay::get()->setUserEnabled(enabled);
}

std::string Speedhack::getText()
{
    return str;
}

void Speedhack::setText(std::string text)
{
    this->str = text;
    Mod::get()->setSavedValue<std::string>("speedhack-top_value", str);

    value = 1.0f;
    value = utils::numFromString<float>(text).unwrapOr(1.0f);

    SafeMode::get()->onSpeedhackChanged();
}

float Speedhack::getRealValue()
{
    if (!getEnabled())
        return 1.0f;

    if (CCScene::get() && CCScene::get()->getChildByType<LoadingLayer>(0))
        return 1.0f;

    return std::min<float>(std::max<float>(value, 0.01f), 999999);
}

bool Speedhack::gameplayOnlyCheck()
{
    if (getGameplayEnabled())
    {
        return GJBaseGameLayer::get();
    }

    return true;
}

float Speedhack::getValue()
{
    return std::min<float>(std::max<float>(value, 0.001f), 999999);
}

void Speedhack::loadPresets()
{
    auto vec = Mod::get()->getSavedValue<std::vector<float>>("speedhack-presets", { 0.1f, 0.25f, 0.3f, 0.50f, 0.75f, 1.0f, 1.50f, 2.0f });
    presets.clear();

    for (auto pre : vec)
    {
        auto struc = SpeedhackPreset(pre);
        presets.push_back(struc);
    }

    auto configsValue = Mod::get()->getSavedValue<matjson::Value>("speedhack-presets-keyconfigs");

    if (configsValue.isArray())
    {
        for (auto config : configsValue.asArray().unwrap())
        {
            for (size_t i = 0; i < presets.size(); i++)
            {
                if (presets[i].value == config["value"].asDouble().unwrap())
                    presets[i].keyConfig.load(config["config"]);
            }
        }
    }
}

void Speedhack::savePresets()
{
    std::vector<float> vec = {};

    auto configsValue = matjson::Value::array();

    for (auto pre : presets)
    {
        vec.push_back(pre.value);

        matjson::Value keyVal;
        keyVal["value"] = pre.value;
        keyVal["config"] = pre.keyConfig.save();

        configsValue.asArray().unwrap().push_back(keyVal);
    }

    Mod::get()->setSavedValue<std::vector<float>>("speedhack-presets", vec);
    Mod::get()->setSavedValue<matjson::Value>("speedhack-presets-keyconfigs", configsValue);
}

void Speedhack::setPresets(std::vector<SpeedhackPreset> presets)
{
    std::sort(presets.begin(), presets.end(), [](SpeedhackPreset a, SpeedhackPreset b)
    {
        return a.value < b.value;
    });

    this->presets = presets;
    savePresets();
}

std::vector<SpeedhackPreset> Speedhack::getPresets()
{
    return presets;
}

bool Speedhack::hasPreset(float value)
{
    for (auto preset : presets)
    {
        if (preset.value == value)
            return true;
    }

    return false;
}

FMOD::ChannelGroup* Speedhack::getMasterChannel()
{
    static FMOD::ChannelGroup* group = nullptr;
    auto fmod = FMODAudioEngine::get();

    if (!fmod->m_system)
        return nullptr;

    if (!group)
        fmod->m_system->getMasterChannelGroup(&group);

    return group;
}

float Speedhack::getRealDeltaTime()
{
    return realDeltatime;
}

std::string SpeedhackEnabled::getNotificationString()
{
    auto str = getUserEnabled() ? "ui/notification-mod-enabled" : "ui/notification-mod-disabled";

    return utils::string::replace(LocalisationManager::get()->getLocalisedString(str), "%s", LocalisationManager::get()->getLocalisedString("categories/speedhack"));
}