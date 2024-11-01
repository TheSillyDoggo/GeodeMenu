#include "LabelCommon.hpp"

matjson::Object LabelEvent::save()
{
    matjson::Object obj;

    obj["type"] = geode::cast::as<int>(type);
    obj["enabled"] = enabled;
    obj["colour.r"] = colour.r;
    obj["colour.g"] = colour.g;
    obj["colour.b"] = colour.b;
    obj["colour.a"] = colour.a;
    obj["fadeIn"] = fadeIn;
    obj["hold"] = hold;
    obj["fadeOut"] = fadeOut;

    return obj;
}

void LabelEvent::load(matjson::Object obj)
{
    if (obj.contains("type") && obj["type"].is_number())
        type = geode::cast::as<LabelEventType>(obj["type"].as_int());

    if (obj.contains("enabled") && obj["enabled"].is_bool())
        enabled = obj["enabled"].as_bool();

    if (obj.contains("colour.r") && obj["colour.r"].is_number())
        colour.r = obj["colour.r"].as_int();

    if (obj.contains("colour.g") && obj["colour.g"].is_number())
        colour.g = obj["colour.g"].as_int();

    if (obj.contains("colour.b") && obj["colour.b"].is_number())
        colour.b = obj["colour.b"].as_int();

    if (obj.contains("colour.a") && obj["colour.a"].is_number())
        colour.a = obj["colour.a"].as_int();

    if (obj.contains("fadeIn") && obj["fadeIn"].is_number())
        fadeIn = obj["fadeIn"].as_double();

    if (obj.contains("hold") && obj["hold"].is_number())
        hold = obj["hold"].as_double();

    if (obj.contains("fadeOut") && obj["fadeOut"].is_number())
        fadeOut = obj["fadeOut"].as_double();
}

bool operator==(const LabelEvent& a, const LabelEvent& b) = default;