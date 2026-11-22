#pragma once

#include "Range.hpp"

namespace qolmod
{
    struct Ranges
    {
        std::vector<Range> ranges = {};

        bool getEnable(float value, bool def, bool* inAnyRange = nullptr);

        void addRange(float min, float max, bool enable);
        void addRange(qolmod::Range range);

        void sort();
        void clear();
        bool isEmpty();

        matjson::Value save();
        void load(const matjson::Value& value);
    };
};