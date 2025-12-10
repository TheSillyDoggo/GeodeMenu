#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ColourUtils
{
    using Channel = std::string;
    friend class SpeedhackScheduler;

    protected:
        std::vector<Channel> channels = {};
        std::unordered_map<Channel, float> values = {};
        std::unordered_map<Channel, float> speeds = {};

        void update(float dt);
        void addChannel(Channel channel);

    public:
        static ColourUtils* get();
        void setup();

        ccColor3B hsvToRgb(const ccHSVValue& hsv);

        void setChannelSpeed(Channel channel, float speed);

        ccColor3B getChroma(Channel channel);
        ccColor3B getPastel(Channel channel);
};