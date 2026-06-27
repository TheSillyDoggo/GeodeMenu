#include "Range.hpp"

using namespace geode::prelude;
using namespace qolmod;

bool Range::inRange(float v)
{
    return v >= min && v <= max;
}

matjson::Value Range::save()
{
    matjson::Value value;
    value["min"] = min;
    value["max"] = max;
    value["enable"] = enable;

    return value;
}

void Range::load(const matjson::Value& value)
{
    min = value["min"].asDouble().unwrapOr(min);
    max = value["max"].asDouble().unwrapOr(max);
    enable = value["enable"].asBool().unwrapOr(enable);
}