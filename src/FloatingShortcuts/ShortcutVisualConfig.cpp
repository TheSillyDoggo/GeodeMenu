#include "ShortcutVisualConfig.hpp"
#include <dbuf/ByteReader.hpp>
#include <dbuf/ByteWriter.hpp>

using namespace geode::prelude;
using namespace qolmod;

constexpr static std::array<uint8_t, 8> FILE_MAGIC = { 'Q','O','L','M','O','D','S','H' };
#define SHORTCUT_FILE_VERSION 1

void ShortcutVisualConfig::loadV1(matjson::Value value, bool enabled, std::string moduleID)
{
    flags.set();

    flags.set(0, value["visibility/in-game"].asBool().unwrapOr(true));
    flags.set(1, value["visibility/in-pause-menu"].asBool().unwrapOr(true));
    flags.set(2, value["visibility/in-menu"].asBool().unwrapOr(true));
    flags.set(3, value["visibility/in-editor"].asBool().unwrapOr(true));
    flags.set(4, value["visibility/in-pause-editor"].asBool().unwrapOr(true));

    flags.set(5, value["movable"].asBool().unwrapOr(true));

    scale = value["scale"].asDouble().unwrapOr(0.8f);
    opacity = value["opacity"].asDouble().unwrapOr(0.8f);
    animation = (FloatingButtonBase::AnimationType)(value["animation"].asInt().unwrapOr(0));

    auto offStr = value["off-bg-sprite"].asString().unwrapOr("geode.loader/baseCircle_Medium_Gray.png");
    auto onStr = value["on-bg-sprite"].asString().unwrapOr("geode.loader/baseCircle_Medium_Green.png");

    offSprite = stringToPair(offStr);
    onSprite = stringToPair(onStr);

    flags.set(6, offStr == "");
    flags.set(7, onStr == "");

    flags.set(8, enabled);
    this->moduleID = moduleID;

    auto overlayStr = value["overlay-sprite"].asString().unwrapOr("");
    overlayType = overlayStr == "" ? OverlayType::None : OverlayType::BuiltIn;
    overlayBuiltinSprite = overlayStr;

    if (value["colour_colour"].isObject())
    {
        overlayColour.customColour.r = value["colour_colour"]["r"].asInt().unwrapOr(255);
        overlayColour.customColour.g = value["colour_colour"]["g"].asInt().unwrapOr(255);
        overlayColour.customColour.b = value["colour_colour"]["b"].asInt().unwrapOr(255);
    }
    overlayColour.opacity = value["colour_opacity"].asDouble().unwrapOr(1);
    overlayColour.chromaSpeed = value["colour_chromaspeed"].asDouble().unwrapOr(1);
    overlayColour.type = (ColourConfigType)value["colour_type"].asInt().unwrapOr(0);
}

void ShortcutVisualConfig::loadV2(std::span<uint8_t> data)
{
    dbuf::ByteReader br{data};

    std::array<uint8_t, 8> magic;
    if (!br.readBytes(magic.data(), 8).isOk() || magic != FILE_MAGIC)
    {
        log::error("File magic doesn't match! incorrect file");
        return;
    }

    auto version = br.readU16().unwrap();

    if (version > SHORTCUT_FILE_VERSION)
    {
        log::error("File version is unsupported: {}>{}", version, SHORTCUT_FILE_VERSION);
        return;
    }

    moduleID = br.readStringU8().unwrap();

    flags = br.readU16().unwrap();
    scale = br.readFloat().unwrap();
    opacity = br.readFloat().unwrap();
    animation = (FloatingButtonBase::AnimationType)br.readU8().unwrap();
    group = br.readU8().unwrap();

    offSprite.first = (ShortcutShape)br.readU8().unwrap();
    offSprite.second = (ShortcutColour)br.readU8().unwrap();

    onSprite.first = (ShortcutShape)br.readU8().unwrap();
    onSprite.second = (ShortcutColour)br.readU8().unwrap();

    overlayType = (OverlayType)br.readU8().unwrap();
    overlayBuiltinSprite = br.readStringU16().unwrap();
    overlayText = br.readStringU16().unwrap();
    overlayFont = br.readStringU16().unwrap();

    std::vector<uint8_t> colourData(br.readU32().unwrap(), 0);
    (void)br.readBytes(colourData.data(), colourData.size());
    overlayColour.loadBytes(colourData);

    colourData.assign(br.readU32().unwrap(), 0);
    (void)br.readBytes(colourData.data(), colourData.size());
    outlineColour.loadBytes(colourData);

    uint64_t customSize = br.readU64().unwrap();
    customOverlayData.resize(customSize);

    if (!br.readBytes(customOverlayData.data(), customSize))
    {
        // not enough data
    }

    log::debug("Loaded shortcut for '{}'", moduleID);
}

std::vector<uint8_t> ShortcutVisualConfig::saveV2()
{
    dbuf::ByteWriter wr;
    wr.writeBytes(FILE_MAGIC.data(), FILE_MAGIC.size());
    wr.writeU16(SHORTCUT_FILE_VERSION);

    wr.writeStringU8(moduleID);

    wr.writeU16(flags.to_ulong());
    wr.writeFloat(scale);
    wr.writeFloat(opacity);
    wr.writeU8((uint8_t)animation);
    wr.writeU8(group);

    wr.writeU8((uint8_t)offSprite.first);
    wr.writeU8((uint8_t)offSprite.second);

    wr.writeU8((uint8_t)onSprite.first);
    wr.writeU8((uint8_t)onSprite.second);

    wr.writeU8((uint8_t)overlayType);
    wr.writeStringU16(overlayBuiltinSprite);
    wr.writeStringU16(overlayText);
    wr.writeStringU16(overlayFont);

    auto cData = overlayColour.saveBytes();
    wr.writeU32(cData.size());
    wr.writeBytes(cData);

    cData = outlineColour.saveBytes();
    wr.writeU32(cData.size());
    wr.writeBytes(cData);

    wr.writeU64(customOverlayData.size());
    wr.writeBytes(customOverlayData);

    return wr.writtenVec();
}

void ShortcutVisualConfig::loadV2(std::filesystem::path path)
{
    auto res = file::readBinary(path);

    if (!res.isOk())
        return;

    flags.set();
    loadV2(res.unwrap());
}

void ShortcutVisualConfig::saveV2(std::filesystem::path path)
{
    auto res = file::writeBinarySafe(path, saveV2());
    
    if (res.isErr())
        log::error("Failed to write ShortcutVisualConfig! {}, path: {}", res.err(), path);
}

ShortcutVisualConfig::SpritePair ShortcutVisualConfig::stringToPair(std::string old)
{
    ShortcutVisualConfig::SpritePair pair;
    pair.first = ShortcutShape::Circle;

    static std::unordered_map<std::string, qolmod::ShortcutColour> colours = {
        { "geode.loader/baseCircle_Medium_Blue.png", qolmod::ShortcutColour::Blue },
        { "geode.loader/baseCircle_Medium_DarkAqua.png", qolmod::ShortcutColour::DarkAqua },
        { "geode.loader/baseCircle_Medium_DarkPurple.png", qolmod::ShortcutColour::DarkPurple },
        { "geode.loader/baseCircle_Medium_Gray.png", qolmod::ShortcutColour::Gray },
        { "geode.loader/baseCircle_Medium_Green.png", qolmod::ShortcutColour::Green },
        { "geode.loader/baseCircle_Medium_Pink.png", qolmod::ShortcutColour::Pink },
    };
    pair.second = colours[old];
    return pair;
}

bool ShortcutVisualConfig::shouldShow()
{
    if (auto pl = PlayLayer::get())
    {
        if (CCScene::get()->getChildByType<PauseLayer>(0) && flags[1])
            return true;

        if (!CCScene::get()->getChildByType<PauseLayer>(0) && flags[0])
            return true;
    }
    else if (auto ed = LevelEditorLayer::get())
    {
        if (ed->getChildByType<EditorPauseLayer>(0) && flags[4])
            return true;

        if (!ed->getChildByType<EditorPauseLayer>(0) && flags[3])
            return true;
    }
    else
    {
        if (flags[2])
            return true;
    }

    return false;
}

// getters
bool ShortcutVisualConfig::isMovable() {
    return flags[5];
}

bool ShortcutVisualConfig::isEnabled() {
    return flags[8];
}

bool ShortcutVisualConfig::hasOffBG() {
    return !flags[6];
}

bool ShortcutVisualConfig::hasOnBG() {
    return !flags[7];
}

ShortcutVisualConfig::SpritePair ShortcutVisualConfig::getOffBG() {
    return offSprite;
}

ShortcutVisualConfig::SpritePair ShortcutVisualConfig::getOnBG() {
    return onSprite;
}

float ShortcutVisualConfig::getScale() {
    return scale;
}

float ShortcutVisualConfig::getOpacity() {
    return opacity;
}

std::string ShortcutVisualConfig::getOverlayText() {
    return overlayText;
}

std::string ShortcutVisualConfig::getOverlayFont() {
    return overlayFont;
}

FloatingButtonBase::AnimationType ShortcutVisualConfig::getAnimation() {
    return animation;
}

ShortcutVisualConfig::OverlayType ShortcutVisualConfig::getOverlayType() {
    return overlayType;
}

std::string ShortcutVisualConfig::getOverlayBuiltIn() {
    return overlayBuiltinSprite;
}

cocos2d::ccColor3B ShortcutVisualConfig::getOutlineColour() {
    return overlayColour.colourForConfig(moduleID);
}

cocos2d::ccColor3B ShortcutVisualConfig::getOverlayColour() {
    return overlayColour.colourForConfig(moduleID);
}

std::string ShortcutVisualConfig::getModuleID() {
    return moduleID;
}