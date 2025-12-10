#include "ColourUtils.hpp"
#include "ColourConfig.hpp"
#include "../GUI/AndroidBall.hpp"

// setup
$execute
{
    ColourUtils::get();
}

ColourUtils* ColourUtils::get()
{
    static ColourUtils* instance = nullptr;

    if (!instance)
    {
        instance = new ColourUtils();
    }

    return instance;
}

void ColourUtils::setup()
{
    
}

void ColourUtils::update(float dt)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        values[channels[i]] += dt * speeds[channels[i]];
    }
}

void ColourUtils::addChannel(Channel channel)
{
    if (values.contains(channel))
        return;

    values.emplace(channel, 0);
    speeds.emplace(channel, 1);
    channels.push_back(channel);
}

ccColor3B ColourUtils::getChroma(Channel channel)
{
    if (!values.contains(channel))
        addChannel(channel);

    return hsvToRgb(cchsv((values[channel] * 180) / 10.0f, 1.0f, 1.0f, true, true));
}

ccColor3B ColourUtils::getPastel(Channel channel)
{
    if (!values.contains(channel))
        addChannel(channel);

    return hsvToRgb(cchsv((values[channel] * 180) / 10.0f, 155.0f / 255.0f, 1.0f, true, true));
}

void ColourUtils::setChannelSpeed(Channel channel, float speed)
{
    addChannel(channel);

    speeds[channel] = speed;
}

cocos2d::ccColor3B ColourConfig::colourForConfig(std::string channel)
{
    switch (type)
    {
        case CustomColour:
            return customColour;

        case Player1:
            return GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value());

        case Player2:
            return GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value());

        case PlayerGlow:
            return GameManager::get()->colorForIdx(GameManager::get()->m_playerGlowColor.value());

        case Chroma:
            return ColourUtils::get()->getChroma(channel);

        case Pastel:
            return ColourUtils::get()->getPastel(channel);
        
        default:
            return ccWHITE;
    }
}

// stolen from somewhere, idk where though :P
ccColor3B ColourUtils::hsvToRgb(const ccHSVValue& hsv)
{
    float hue = hsv.h;
    float saturation = hsv.s;
    float value = hsv.v;

    int hi = static_cast<int>(std::floor(hue / 60.0f)) % 6;
    float f = hue / 60.0f - std::floor(hue / 60.0f);

    float p = value * (1 - saturation);
    float q = value * (1 - f * saturation);
    float t = value * (1 - (1 - f) * saturation);

    float r, g, b;

    switch (hi) {
        case 0: r = value; g = t; b = p; break;
        case 1: r = q; g = value; b = p; break;
        case 2: r = p; g = value; b = t; break;
        case 3: r = p; g = q; b = value; break;
        case 4: r = t; g = p; b = value; break;
        case 5: r = value; g = p; b = q; break;
        default: r = g = b = 0; break;
    }

    return ccc3(static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255));
}