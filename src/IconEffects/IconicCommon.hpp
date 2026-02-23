#pragma once

#include <string>

enum class IconicGamemodeType
{
    Cube = 1,
    Ship = 2,
    Jetpack = 3,
    Ball = 4,
    Ufo = 5,
    Wave = 6,
    Robot = 7,
    Spider = 8,
    Swing = 9,

    Bird = 5,
    Dart = 6,
};

enum class IconicEffectType
{
    Primary = 1,
    Secondary = 2,
    Glow = 3,
    Trail = 4,
    Ghost = 5,
    WaveTrail = 6,
    FineOutline = 7,
};

enum class IconicDualMode
{
    Invert = 0,
    Same = 1,
    Seperate = 2,
};