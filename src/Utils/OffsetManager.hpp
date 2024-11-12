#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum class PatchType
{
    PhysicsBypass,
};

class OffsetManager
{
    public:
        static OffsetManager* get();

        unsigned int offsetForType(PatchType type);
        unsigned int offsetForRandomSeed();
};