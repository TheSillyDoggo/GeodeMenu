#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    struct Range
    {
        float min = 0;
        float max = 1;
        bool enable = true;

        Range() = default;
        Range(float min, float max, float enable);

        bool inRange(float v);

        matjson::Value save();
        void load(const matjson::Value& value);
    };
};