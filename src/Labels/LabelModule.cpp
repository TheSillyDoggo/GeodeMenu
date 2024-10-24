#include "LabelModule.hpp"
#include "Labels.h"
#include "LabelLayer.hpp"
#include "../Hacks/SafeMode/SafeMode.hpp"

LabelModule::LabelModule(std::string format, std::string font)
{
    this->format = format;
    this->font = font;
}

void LabelModule::setFont(std::string newFont)
{
    this->font = newFont;

    if (labelNode)
        labelNode->setFntFile(getFont().c_str());
}

std::string LabelModule::getFont()
{
    return CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(font.c_str(), false)) ? font : "bigFont.fnt";
}

void LabelModule::setScale(float newScale)
{
    this->scale = newScale;

    if (labelNode)
    {
        labelNode->setScale(newScale * 0.5f);
        labelNode->getParent()->updateLayout();
    }
}

float LabelModule::getScale()
{
    return scale;
}

void LabelModule::setOpacity(float newOpacity)
{
    this->opacity = newOpacity;

    if (labelNode)
    {
        labelNode->setOpacity(opacity * 255.0f);
    }
}

float LabelModule::getOpacity()
{
    return opacity;
}

ccColor3B LabelModule::getColour()
{
    if (isCheatIndicator)
        return SafeMode::get()->colourForState();

    return ccWHITE;
}

void LabelModule::setSide(LabelAnchor newSide)
{
    this->side = newSide;

    if (LabelLayer::get())
        LabelLayer::get()->updateAnchors();
}

LabelAnchor LabelModule::getSide()
{
    return side;
}

matjson::Object LabelModule::saveToObject()
{
    matjson::Object obj;

    obj["display_name"] = this->name;
    obj["format"] = this->format;
    obj["scale"] = this->scale;
    obj["opacity"] = this->opacity;
    obj["font"] = this->font;
    obj["side"] = as<int>(this->side);
    obj["offset.x"] = offset.x;
    obj["offset.y"] = offset.y;
    obj["preset"] = presetType;
    obj["cheat_indicator"] = isCheatIndicator;
    obj["noclip_only"] = noclipOnly;

    matjson::Array eventsArr;
    
    for (auto event : events)
    {
        eventsArr.push_back(event.save());
    }

    obj["events"] = eventsArr;

    return obj;
}

LabelModule* LabelModule::createFromObject(matjson::Object obj)
{
    auto mod = new LabelModule("", "bigFont.fnt");

    if (obj.contains("display_name") && obj["display_name"].is_string())
        mod->name = obj["display_name"].as_string();
    else
        mod->name = "Error Loading Label";

    if (obj.contains("format") && obj["format"].is_string())
        mod->format = obj["format"].as_string();

    if (obj.contains("scale") && obj["scale"].is_number())
        mod->scale = obj["scale"].as_double();

    if (obj.contains("opacity") && obj["opacity"].is_number())
        mod->opacity = obj["opacity"].as_double();

    if (obj.contains("font") && obj["font"].is_string())
        mod->font = obj["font"].as_string();

    if (obj.contains("side") && obj["side"].is_number())
        mod->side = as<LabelAnchor>(obj["side"].as_int());

    if (obj.contains("offset.x") && obj["offset.x"].is_number())
        mod->offset.x = obj["offset.x"].as_double();

    if (obj.contains("offset.y") && obj["offset.y"].is_number())
        mod->offset.y = obj["offset.y"].as_double();

    if (obj.contains("preset") && obj["preset"].is_number())
        mod->presetType = obj["preset"].as_int();

    if (obj.contains("cheat_indicator") && obj["cheat_indicator"].is_bool())
        mod->isCheatIndicator = obj["cheat_indicator"].as_bool();

    if (obj.contains("noclip_only") && obj["noclip_only"].is_bool())
        mod->noclipOnly = obj["noclip_only"].as_bool();

    if (obj.contains("events") && obj["events"].is_array())
    {
        for (auto obj : obj["events"].as_array())
        {
            if (!obj.is_object())
                continue;

            LabelEvent event;
            event.load(obj.as_object());

            mod->events.push_back(event);
        }
    }

    return mod;
}