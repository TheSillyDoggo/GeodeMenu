#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ColourUtils
{
    using Channel = int;

    protected:
        std::unordered_map<Channel, float> values = {};
        std::unordered_map<Channel, float> speeds = {};

        void setup();
        void update(float dt);
        void addChannel(Channel channel);

    public:
        static ColourUtils* get();

        ccColor3B hsvToRgb(const ccHSVValue& hsv);

        ccColor3B getChroma(Channel channel);
        ccColor3B getPastel(Channel channel);
};