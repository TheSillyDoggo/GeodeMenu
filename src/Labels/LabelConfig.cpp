#include "LabelConfig.hpp"

LabelConfig LabelConfig::createFromObject(matjson::Value obj)
{
    LabelConfig conf;

    if (obj.contains("display_name") && obj["display_name"].isString())
        conf.displayName = obj["display_name"].asString().unwrap();
    else
        conf.displayName = "Error Loading Label";

    if (obj.contains("format") && obj["format"].isString())
        conf.formatString = obj["format"].asString().unwrap();

    if (obj.contains("scale") && obj["scale"].isNumber())
        conf.scale = obj["scale"].asDouble().unwrap();

    if (obj.contains("opacity") && obj["opacity"].isNumber())
        conf.opacity = obj["opacity"].asDouble().unwrap();

    if (obj.contains("font") && obj["font"].isString())
        conf.font = obj["font"].asString().unwrap();

    if (obj.contains("side") && obj["side"].isNumber())
        conf.anchor = (LabelAnchor)obj["side"].asInt().unwrap();

    if (obj.contains("offset.x") && obj["offset.x"].isNumber())
        conf.offset.x = obj["offset.x"].asDouble().unwrap();

    if (obj.contains("offset.y") && obj["offset.y"].isNumber())
        conf.offset.y = obj["offset.y"].asDouble().unwrap();

    if (obj.contains("cheat_indicator") && obj["cheat_indicator"].isBool())
        conf.cheatIndicator= obj["cheat_indicator"].asBool().unwrap();

    if (obj.contains("noclip_only") && obj["noclip_only"].isBool())
        conf.noclipOnly = obj["noclip_only"].asBool().unwrap();

    if (obj.contains("visible") && obj["visible"].isBool())
        conf.visible = obj["visible"].asBool().unwrap();

    if (obj.contains("label_type") && obj["label_type"].isNumber())
        conf.type = (LabelType)obj["label_type"].asInt().unwrap();

    /*
    if (obj.contains("events") && obj["events"].isArray())
    {
        for (auto obj : obj["events"].asArray().unwrap())
        {
            LabelEvent event;
            event.load(obj);

            mod->events.push_back(event);
        }
    }
        */

    return conf;
}