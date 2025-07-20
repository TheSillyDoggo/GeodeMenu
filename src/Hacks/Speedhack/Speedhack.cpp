#include "Speedhack.hpp"
#include <Geode/Geode.hpp>
#include "../../SafeMode/SafeMode.hpp"

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
    setEnabled(Mod::get()->getSavedValue<bool>("speedhack-enabled_enabled", false));
    setMusicEnabled(Mod::get()->getSavedValue<bool>("speedhack-mus_enabled", true));
    setGameplayEnabled(Mod::get()->getSavedValue<bool>("speedhack-gameplay_enabled", false));
    setText(Mod::get()->getSavedValue<std::string>("speedhack-top_value", "1.0"));
}

bool Speedhack::getEnabled()
{
    return enabled;
}

void Speedhack::setEnabled(bool value)
{
    this->enabled = value;

    Mod::get()->setSavedValue<bool>("speedhack-enabled_enabled", value);

    SafeMode::get()->onSpeedhackChanged();
}

bool Speedhack::getMusicEnabled()
{
    return musicEnabled;
}

void Speedhack::setMusicEnabled(bool enabled)
{
    this->musicEnabled = enabled;

    Mod::get()->setSavedValue<bool>("speedhack-mus_enabled", enabled);
}

bool Speedhack::getGameplayEnabled()
{
    return gameplayEnabled;
}

void Speedhack::setGameplayEnabled(bool enabled)
{
    gameplayEnabled = enabled;

    Mod::get()->setSavedValue<bool>("speedhack-gameplay_enabled", enabled);
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

    return std::min<float>(std::max<float>(value, 0.001f), 999999);
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

FMOD::ChannelGroup* Speedhack::getMasterChannel()
{
    static FMOD::ChannelGroup* group = nullptr;

    if (!group)
        FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&group);

    return group;
}