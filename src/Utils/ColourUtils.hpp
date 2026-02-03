#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// #define QOLMOD_SEPERATE_COLOURS

class ColourUtils
{
    using Channel = std::string;
    friend class SpeedhackScheduler;

    protected:
        std::vector<Channel> channels = {};
        float globalValue = 0;
        std::unordered_map<Channel, float> values = {};
        std::unordered_map<Channel, float> speeds = {};

        void update(float dt);
        void addChannel(Channel channel);

    public:
        static ColourUtils* get();
        void setup();

        ccColor3B hsvToRgb(const ccHSVValue& hsv);
        ccColor3B lerpColour(const ccColor3B& color1, const ccColor3B& color2, float t);

        void setChannelSpeed(Channel channel, float speed);
        float getLoopedValue(float value);

        ccColor3B getChroma(Channel channel);
        ccColor3B getPastel(Channel channel);
        float getChannelValue(Channel channel);
};