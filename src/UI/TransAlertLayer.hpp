#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class TransAlertLayer : public FLAlertLayer
{
    public:
        bool init(char const* title, const gd::string& desc, char const* btn);

        static TransAlertLayer* create(char const* title, const gd::string& desc, char const* btn);
};