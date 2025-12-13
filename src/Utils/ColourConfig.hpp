#pragma once

enum ColourConfigType
{
    CustomColour,
    Player1,
    Player2,
    PlayerGlow,
    Chroma,
    Pastel,
    Gradient,
};

struct ColourConfig
{
    struct GradientLocation
    {
        cocos2d::ccColor3B colour = cocos2d::ccc3(255, 255, 255);
        float percentageLocation = 0.0f;
    };

    cocos2d::ccColor3B customColour = cocos2d::ccc3(255, 255, 255);
    float opacity = 1.0f;
    float chromaSpeed = 1.0f;
    ColourConfigType type = CustomColour;
    std::vector<GradientLocation> gradientLocations = { { cocos2d::ccc3(255, 0, 0), 0 }, { cocos2d::ccc3(0, 255, 0), 1 } };

    cocos2d::ccColor3B colourForConfig(std::string channel);
    cocos2d::ccColor3B colourForGradient(float v);
};