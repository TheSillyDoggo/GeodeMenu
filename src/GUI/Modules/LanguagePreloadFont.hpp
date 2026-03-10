#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class LanguagePreloadFont : public Module
{
    public:
        MODULE_SETUP(LanguagePreloadFont)
        {
            setID("preload-language-font");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(LanguagePreloadFont);