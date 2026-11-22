#include "Ranges.hpp"
#include <math.h>

using namespace geode::prelude;
using namespace qolmod;

// purpose: merge overlapped ranges
void Ranges::sort()
{
    return;
    std::unordered_map<float, uint8_t> commandMap = {};

    std::function<bool(float)> canReplace = [&](float v)
    {
        if (commandMap.contains(v) && commandMap[v] == 0)
            return true;

        return !commandMap.contains(v);
    };

    for (auto& zone : ranges)
    {
        if (canReplace(zone.min))
            commandMap.emplace(zone.min, zone.enable ? 1 : 2);
        
        if (canReplace(zone.max))
            commandMap.emplace(zone.max, 0);
    }

    std::map<float, uint8_t> sorted(commandMap.begin(), commandMap.end());
    log::info("BEGIN");

    for (auto& sort : sorted)
    {
        log::error("1: {}, 2: {}", sort.first, sort.second);
    }
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
    if (fabs(range.min - range.max) <= 0.00005f)
        return;

    ranges.insert(ranges.begin(), range);
    sort();
}

void Ranges::addRange(float min, float max, bool enable)
{
    if (fabs(min - max) <= 0.00005f)
        return;

    ranges.insert(ranges.begin(), qolmod::Range(min, max, enable));
    sort();
}

void Ranges::clear()
{
    ranges.clear();
}

bool Ranges::isEmpty()
{
    return ranges.empty();
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