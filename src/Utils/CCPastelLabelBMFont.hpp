#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class CCPastelLabelBMFont : public CCLabelBMFont
{
    public:
        static CCPastelLabelBMFont* create(const char* text, const char* fontFile);
        void update(float dt);

    protected:
        CCPastelLabelBMFont();
        virtual ~CCPastelLabelBMFont();

        bool initWithString(const char* text, const char* fontFile);
};