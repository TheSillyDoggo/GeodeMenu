#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    struct Range
    {
        double min = 0;
        double max = 1;
        bool enable = true;

        bool inRange(float v);

        matjson::Value save();
        void load(const matjson::Value& value);
    };
};