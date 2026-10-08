#include "ColourConfig.hpp"
#include <dbuf/ByteReader.hpp>
#include <dbuf/ByteWriter.hpp>

using namespace geode::prelude;

constexpr static std::array<uint8_t, 2> FILE_MAGIC = { 'Q','C' };
#define COLCONF_FILE_VERSION 1

void ColourConfig::loadBytes(std::vector<uint8_t> data)
{
    dbuf::ByteReader br{data};

    std::array<uint8_t, 2> magic;
    if (!br.readBytes(magic.data(), 2).isOk() || magic != FILE_MAGIC)
    {
        log::error("ColourConfig magic doesn't match! incorrect file");
        return;
    }

    auto version = br.readU16().unwrap();

    if (version > COLCONF_FILE_VERSION)
    {
        log::error("ColourConfig version is unsupported: {}>{}", version, COLCONF_FILE_VERSION);
        return;
    }

    type = (ColourConfigType)br.readU8().unwrap();
    std::bitset<8> flags = br.readU8().unwrap();
    smoothGradient = flags[0];
    inverted = flags[1];
    loopGradient = flags[2];

    customColour = ccc3(
        br.readU8().unwrap(),
        br.readU8().unwrap(),
        br.readU8().unwrap()
    );

    opacity = br.readFloat().unwrap();
    chromaSpeed = br.readFloat().unwrap();

    auto gradientCounts = br.readU32().unwrap();
    gradientLocations.clear();
    for (size_t i = 0; i < gradientCounts; i++)
    {
        gradientLocations.push_back({
            ccc3(
                br.readU8().unwrap(),
                br.readU8().unwrap(),
                br.readU8().unwrap()
            ), br.readFloat().unwrap()
        });
    }

    if (br.position() != data.size())
    {
        log::error("Position != size ({} != {})", br.position(), data.size());
    }
}

std::vector<uint8_t> ColourConfig::saveBytes()
{
    dbuf::ByteWriter wr;

    wr.writeBytes(FILE_MAGIC.data(), FILE_MAGIC.size());
    wr.writeU16(COLCONF_FILE_VERSION);

    wr.writeU8((uint8_t)type);

    std::bitset<8> flags;
    flags.set(0, smoothGradient);
    flags.set(1, inverted);
    flags.set(2, loopGradient);
    wr.writeU8((uint8_t)flags.to_ulong());

    wr.writeU8(customColour.r);
    wr.writeU8(customColour.g);
    wr.writeU8(customColour.b);

    wr.writeFloat(opacity);
    wr.writeFloat(chromaSpeed);

    wr.writeU32(gradientLocations.size());
    for (auto& gl : gradientLocations)
    {
        wr.writeU8(gl.colour.r);
        wr.writeU8(gl.colour.g);
        wr.writeU8(gl.colour.b);
        wr.writeFloat(gl.percentageLocation);
    }
    
    return wr.writtenVec();
}