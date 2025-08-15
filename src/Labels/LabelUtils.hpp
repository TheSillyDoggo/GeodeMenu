#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LabelUtils
{
    public:
        static LabelUtils* get();

        CCRect getSafeZone();
};