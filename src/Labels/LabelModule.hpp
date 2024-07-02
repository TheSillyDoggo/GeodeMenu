#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class LabelModule : public Module
{
    public:
        std::string format;

        LabelModule(std::string format);
};