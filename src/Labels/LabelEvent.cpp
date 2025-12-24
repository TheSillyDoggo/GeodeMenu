#include "LabelAnchor.hpp"

matjson::Value LabelEvent::save()
{
    matjson::Value obj;

    obj["type"] = (int)type;
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

void LabelEvent::load(matjson::Value obj)
{
    if (obj.contains("type") && obj["type"].isNumber())
        type = (LabelEventType)obj["type"].asInt().unwrap();

    if (obj.contains("enabled") && obj["enabled"].isBool())
        enabled = obj["enabled"].asBool().unwrap();

    if (obj.contains("colour.r") && obj["colour.r"].isNumber())
        colour.r = obj["colour.r"].asInt().unwrap();

    if (obj.contains("colour.g") && obj["colour.g"].isNumber())
        colour.g = obj["colour.g"].asInt().unwrap();

    if (obj.contains("colour.b") && obj["colour.b"].isNumber())
        colour.b = obj["colour.b"].asInt().unwrap();

    if (obj.contains("colour.a") && obj["colour.a"].isNumber())
        colour.a = obj["colour.a"].asInt().unwrap();

    if (obj.contains("fadeIn") && obj["fadeIn"].isNumber())
        fadeIn = obj["fadeIn"].asDouble().unwrap();

    if (obj.contains("hold") && obj["hold"].isNumber())
        hold = obj["hold"].asDouble().unwrap();

    if (obj.contains("fadeOut") && obj["fadeOut"].isNumber())
        fadeOut = obj["fadeOut"].asDouble().unwrap();
}

bool operator==(const LabelEvent& a, const LabelEvent& b) = default;