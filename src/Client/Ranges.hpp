#pragma once

#include "Range.hpp"

namespace qolmod
{
    struct Ranges
    {
        std::vector<Range> ranges = {};

        void sort();

        bool getEnable(float value, bool def);

        matjson::Value save();
        void load(const matjson::Value& value);
    };
};