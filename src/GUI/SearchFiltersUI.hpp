#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"

using namespace geode::prelude;

class SearchFiltersUI : public geode::Popup<>
{
    protected:
        BackgroundSprite* bg;

    public:
        static SearchFiltersUI* create();

        virtual bool setup();
};