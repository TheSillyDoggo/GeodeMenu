#include "RangeManager.hpp"
#include <dbuf/ByteReader.hpp>
#include <dbuf/ByteWriter.hpp>
#include "Module.hpp"

using namespace geode::prelude;
using namespace qolmod;

RangeManager* RangeManager::get()
{
    static RangeManager instance;
    return &instance;
}

void RangeManager::clearEmpty()
{
    std::erase_if(m_configs, [](const auto& item) {
        return item.second.m_ranges.ranges.size() == 0;
    });
}

ModRangeConfig* RangeManager::getConfig(const std::string& key)
{
    return &m_configs[key];
}

std::string RangeManager::getKey(Module* module, GJGameLevel* level)
{
    if (!level)
        return fmt::format("{}_global", module->getID());

    switch (level->m_levelType)
    {
        case GJLevelType::Main:
            return fmt::format("{}_main{}", module->getID(), level->m_levelID);
        case GJLevelType::Editor:
            return fmt::format("{}_editor{}", module->getID(), level->m_levelName);
        case GJLevelType::Saved:
            return fmt::format("{}_saved{}", module->getID(), level->m_levelID);
        case GJLevelType::Default:
        default:
            return fmt::format("{}_def{}", module->getID(), level->m_levelID);
    }
}

const ModRangeMode& RangeManager::getMode(Module* module)
{
    return m_modes[module];
}

void RangeManager::setMode(Module* module, const ModRangeMode& mode)
{
    if (m_modes[module] == mode)
        return;

    m_modes[module] = mode;
    m_needsSave = true;
}

constexpr static std::array<uint8_t, 5> FILE_MAGIC = { 'R','G','P','A','K' };
#define RANGEDATA_FILE_VERSION 1

void RangeManager::save()
{
    if (!m_needsSave)
        return;
    m_needsSave = false;
    clearEmpty();

    dbuf::ByteWriter wr;
    wr.writeBytes(FILE_MAGIC.data(), FILE_MAGIC.size());
    wr.writeU16(RANGEDATA_FILE_VERSION);
    wr.writeU32(m_configs.size());

    for (auto& conf : m_configs)
    {
        wr.writeStringU16(conf.first);
        wr.writeBool(conf.second.m_enabled);
        wr.writeBool(conf.second.m_notify);
        wr.writeU16(conf.second.m_ranges.ranges.size());

        for (auto& range : conf.second.m_ranges.ranges)
        {
            wr.writeBool(range.enable);
            wr.writeFloat(range.min);
            wr.writeFloat(range.max);
        }
    }

    wr.writeU32(m_modes.size());
    for (auto& mode : m_modes)
    {
        wr.writeStringU8(mode.first->getID());
        wr.writeU8((uint8_t)mode.second);
    }

    auto res = file::writeBinary(Mod::get()->getSaveDir() / "ranges.rgpak", wr.written());
}

void RangeManager::load()
{
    auto res = file::readBinary(Mod::get()->getSaveDir() / "ranges.rgpak");

    if (!res.isOk())
    {
        m_needsSave = true;
        return;
    }

    dbuf::ByteReader br{res.unwrap()};

    std::array<uint8_t, 5> magic;
    if (!br.readBytes(magic.data(), 5).isOk() || magic != FILE_MAGIC)
    {
        log::error("File magic doesn't match! incorrect file");
        return;
    }

    auto version = br.readU16().unwrap();
    auto count = br.readU32().unwrap();

    for (size_t i = 0; i < count; i++)
    {
        auto key = br.readStringU16().unwrap();
        auto conf = getConfig(key);
        conf->m_enabled = br.readBool().unwrap();
        conf->m_notify = br.readBool().unwrap();

        auto count2 = br.readU16().unwrap();
        for (size_t y = 0; y < count2; y++)
        {
            auto enable = br.readBool().unwrap();
            auto min = br.readFloat().unwrap();
            auto max = br.readFloat().unwrap();

            conf->m_ranges.ranges.push_back(Range(min, max, enable));
        }

        conf->m_ranges.sort();
    }

    count = br.readU32().unwrap();
    for (size_t i = 0; i < count; i++)
    {
        auto mod = br.readStringU8().unwrap();
        auto mode = (ModRangeMode)br.readU8().unwrap();

        m_modes[Module::getByID(mod)] = mode;
    }
}

$on_game(Loaded)
{
    RangeManager::get()->load();
    RangeManager::get()->save();
}

$on_mod(DataSaved)
{
    RangeManager::get()->save();
}