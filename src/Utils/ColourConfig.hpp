#pragma once

enum ColourConfigType
{
    CustomColour,
    Player1,
    Player2,
    PlayerGlow,
    Chroma,
    Pastel,
    Fade,
};

struct ColourConfig
{
    cocos2d::ccColor3B customColour = cocos2d::ccc3(255, 255, 255);
    float opacity = 1.0f;
    float chromaSpeed = 1.0f;
    ColourConfigType type = CustomColour;

    cocos2d::ccColor3B colourForConfig(std::string channel);
};