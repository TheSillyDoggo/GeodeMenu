#include "LabelModule.hpp"
#include "Labels.h"
#include "LabelLayer.hpp"

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
}

float LabelModule::getOpacity()
{
    return opacity;
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
    obj["opacity"] = this->scale;
    obj["font"] = this->font;
    obj["side"] = as<int>(this->side);
    obj["offset.x"] = offset.x;
    obj["offset.y"] = offset.y;
    obj["preset"] = presetType;

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

    return mod;
}