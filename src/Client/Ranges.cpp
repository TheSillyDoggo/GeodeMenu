#include "Ranges.hpp"

using namespace geode::prelude;
using namespace qolmod;

void Ranges::sort()
{
    // TODO: implement
    // purpose: stop overlapped ranges
}

bool Ranges::getEnable(float value, bool def, bool* inAnyRange)
{
    *inAnyRange = false;

    for (auto& range : ranges)
    {
        if (range.inRange(value))
        {
            *inAnyRange = true;
            return range.enable;
        }
    }

    return def;
}

void Ranges::addRange(qolmod::Range range)
{
    ranges.push_back(range);
    sort();
}

void Ranges::addRange(float min, float max, bool enable)
{
    ranges.push_back(qolmod::Range({
        .min = min,
        .max = max,
        .enable = enable
    }));
    sort();
}

void Ranges::clear()
{
    ranges.clear();
}

matjson::Value Ranges::save()
{
    sort();
    matjson::Value value = matjson::Value::array();

    for (auto& range : ranges)
    {
        value.push(range.save());
    }

    return value;
}

void Ranges::load(const matjson::Value& value)
{
    ranges.clear();

    if (!value.isArray())
        return;

    for (auto& v : value.asArray().unwrap())
    {
        Range range;
        range.load(v);
        ranges.push_back(range);
    }

    sort();
}