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

float ColourUtils::getChannelValue(Channel channel)
{
    if (values.contains(channel))
        return values[channel];

    return 0;
}

ccColor3B ColourConfig::colourForConfig(std::string channel)
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

        case Gradient:
            return colourForGradient(ColourUtils::get()->getLoopedValue(ColourUtils::get()->getChannelValue(channel)));
        
        default:
            return ccWHITE;
    }
}

ccColor3B ColourConfig::colourForGradient(float v)
{
    if (gradientLocations.size() == 1)
        return gradientLocations[0].colour;

    if (gradientLocations.size() >= 2)
    {
        auto gl = gradientLocations;
        std::sort(gl.begin(), gl.end(), [](GradientLocation a, GradientLocation b)
        {
            return a.percentageLocation < b.percentageLocation;
        });

        for (size_t i = 1; i < gl.size(); i++)
        {
            GradientLocation prev = gl[i - 1];
            GradientLocation cur = gl[i];

            float va = cur.percentageLocation - prev.percentageLocation;

            if (v >= prev.percentageLocation && v <= cur.percentageLocation)
            {
                return ColourUtils::get()->lerpColour(prev.colour, cur.colour, (v - prev.percentageLocation) / va);
            }
        }

        if (gl[0].percentageLocation > v)
            return gl[0].colour;

        if (gl[gl.size() - 1].percentageLocation < v)
            return gl[gl.size() - 1].colour;
    }

    return ccWHITE;
}

float ColourUtils::getLoopedValue(float value)
{
    return (sinf(value) + 1) / 2;
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

ccColor3B ColourUtils::lerpColour(const ccColor3B& color1, const ccColor3B& color2, float t)
{
    if (t < 0)
        t = 0;

    if (t > 1)
        t = 1;

    return ccc3(
        static_cast<uint8_t>(color1.r + (color2.r - color1.r) * t),
        static_cast<uint8_t>(color1.g + (color2.g - color1.g) * t),
        static_cast<uint8_t>(color1.b + (color2.b - color1.b) * t)
    );
}