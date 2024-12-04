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

matjson::Value LabelModule::saveToObject()
{
    matjson::Value obj;

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
    obj["visible"] = visible;

    matjson::Value eventsArr = obj.array();
    
    for (auto event : events)
    {
        eventsArr.asArray().unwrap().push_back(event.save());
    }

    obj["events"] = eventsArr;

    return obj;
}

LabelModule* LabelModule::createFromObject(matjson::Value obj)
{
    auto mod = new LabelModule("", "bigFont.fnt");

    if (obj.contains("display_name") && obj["display_name"].isString())
        mod->name = obj["display_name"].asString().unwrap();
    else
        mod->name = "Error Loading Label";

    if (obj.contains("format") && obj["format"].isString())
        mod->format = obj["format"].asString().unwrap();

    if (obj.contains("scale") && obj["scale"].isNumber())
        mod->scale = obj["scale"].asDouble().unwrap();

    if (obj.contains("opacity") && obj["opacity"].isNumber())
        mod->opacity = obj["opacity"].asDouble().unwrap();

    if (obj.contains("font") && obj["font"].isString())
        mod->font = obj["font"].asString().unwrap();

    if (obj.contains("side") && obj["side"].isNumber())
        mod->side = as<LabelAnchor>(obj["side"].asInt().unwrap());

    if (obj.contains("offset.x") && obj["offset.x"].isNumber())
        mod->offset.x = obj["offset.x"].asDouble().unwrap();

    if (obj.contains("offset.y") && obj["offset.y"].isNumber())
        mod->offset.y = obj["offset.y"].asDouble().unwrap();

    if (obj.contains("preset") && obj["preset"].isNumber())
        mod->presetType = obj["preset"].asInt().unwrap();

    if (obj.contains("cheat_indicator") && obj["cheat_indicator"].isBool())
        mod->isCheatIndicator = obj["cheat_indicator"].asBool().unwrap();

    if (obj.contains("noclip_only") && obj["noclip_only"].isBool())
        mod->noclipOnly = obj["noclip_only"].asBool().unwrap();

    if (obj.contains("visible") && obj["visible"].isBool())
        mod->visible = obj["visible"].asBool().unwrap();

    if (obj.contains("events") && obj["events"].isArray())
    {
        for (auto obj : obj["events"].asArray().unwrap())
        {
            LabelEvent event;
            event.load(obj);

            mod->events.push_back(event);
        }
    }

    return mod;
}

void LabelModule::exportToFile()
{
    auto object = saveToObject();

    auto dump = object.dump();

    file::FilePickOptions options;

    file::FilePickOptions::Filter filter;
    filter.description = "QOLMod Label";
    filter.files = { "*.qollbl" };

    options.filters.push_back(filter);

    file::pick(file::PickMode::SaveFile, options).listen([this, dump](Result<std::filesystem::path>* path)
    {
        if (path->isOk())
        {
            auto filePath = path->unwrapOr(Mod::get()->getConfigDir());

            if (!filePath.has_extension())
                filePath += ".qollbl";
            
            auto res = file::writeString(filePath, dump);

            if (res.isOk())
                FLAlertLayer::create("Success!", "<cg>Success</c> exporting <cc>file</c>!", "OK")->show();
            else
                FLAlertLayer::create("Failure!", fmt::format("<cr>Failed</c> exporting <cc>file</c>!\n<cr>{}</c>", res.unwrapErr()), "OK")->show();
        }
    });
}