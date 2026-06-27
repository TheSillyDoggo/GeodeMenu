#include "Ranges.hpp"

using namespace geode::prelude;
using namespace qolmod;

void Ranges::sort()
{

}

bool Ranges::getEnable(float value, bool def)
{
    for (auto& range : ranges)
    {
        if (range.inRange(value))
            return range.enable;
    }

    return def;
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