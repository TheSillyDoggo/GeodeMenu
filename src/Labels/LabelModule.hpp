#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

enum LabelSide
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    BottomCenter,
};

class LabelModule : public Module
{
    private:
        float scale = 1;
        float opacity = 1;
        std::string font = "bigFont.fnt";
        LabelSide side = LabelSide::TopLeft;
    
    public:
        CCLabelBMFont* labelNode;
        std::string format;

        LabelModule(std::string format, std::string font);

        void setFont(std::string newFont);
        std::string getFont();

        void setScale(float newScale);
        float getScale();

        void setOpacity(float newOpacity);
        float getOpacity();

        void setSide(LabelSide newSide);
        LabelSide getSide();
};