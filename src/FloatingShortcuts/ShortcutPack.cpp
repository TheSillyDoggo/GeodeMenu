#include "ShortcutPack.hpp"
#include <Geode/Geode.hpp>
#include <dbuf/ByteReader.hpp>
#include <dbuf/ByteWriter.hpp>
#include "ShortcutVisualConfig.hpp"

using namespace geode::prelude;

constexpr static std::array<uint8_t, 5> FILE_MAGIC = { 'S','H','P','A','K' };
#define SHPAK_FILE_VERSION 1

void ShortcutPack::save(std::filesystem::path path)
{
    dbuf::ByteWriter wr;
    wr.writeBytes(FILE_MAGIC.data(), FILE_MAGIC.size());
    wr.writeU16(SHPAK_FILE_VERSION);

    wr.writeU32(configs.size());

    for (auto& conf : configs)
    {
        auto data = conf->saveV2();
        wr.writeU32(data.size());
        wr.writeBytes(data.data(), data.size());
    }
    
    auto res = file::writeBinarySafe(path, wr.written());
    
    if (res.isErr())
        log::error("Failed to write ShortcutPack! {}, path: {}", res.err(), path);
}

void ShortcutPack::load(std::filesystem::path path, std::function<void(std::vector<uint8_t>)> readCallback)
{
    auto res = file::readBinary(path);

    if (!res.isOk())
        return;

    dbuf::ByteReader br{res.unwrap()};

    std::array<uint8_t, 5> magic;
    if (!br.readBytes(magic.data(), 5).isOk() || magic != FILE_MAGIC)
    {
        log::error("File magic doesn't match! incorrect file");
        return;
    }

    auto version = br.readU16().unwrap();

    if (version > SHPAK_FILE_VERSION)
    {
        log::error("Pack version is unsupported: {}>{}", version, SHPAK_FILE_VERSION);
        return;
    }

    auto count = br.readU32().unwrap();

    for (size_t i = 0; i < count; i++)
    {
        std::vector<uint8_t> data(br.readU32().unwrap(), 0);
        (void)br.readBytes(data.data(), data.size());
        
        readCallback(data);
    }
    
    log::debug("Loaded shortcut pack with {} shortcuts", count);
}

void ShortcutPack::addConfig(ShortcutVisualConfig* conf)
{
    configs.push_back(conf);
}