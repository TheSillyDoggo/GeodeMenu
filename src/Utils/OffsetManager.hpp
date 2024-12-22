#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum class PatchType
{
    PhysicsBypass,
    EditorExtension1,
    EditorExtension2,
};

enum class FunctionType
{
    FMOD__ChannelControl__removeDSP,
    FMOD__System__createDSPByType,
    FMOD__ChannelControl__addDSP,
    FMOD__DSP__setParameterFloat,
    FMOD__ChannelControl__setPitch,
    FMOD__System__createChannelGroup,
};

class OffsetManager
{
    public:
        static OffsetManager* get();

        unsigned int offsetForType(PatchType type);
        unsigned int offsetForRandomSeed();
        unsigned int offsetForFunction(FunctionType type);
};