#pragma once

#include <Geode/Geode.hpp>
#include "LabelCommon.hpp"
#include "../Client/Module.h"

using namespace geode::prelude;

struct LabelEvent
{
    bool enabled = false;
    ccColor3B colour = ccc3(0, 255, 0);
    float time = 0.5f;
};

class LabelModule : public Module
{
    private:
        float scale = 1;
        float opacity = 1;
        std::string font = "bigFont.fnt";
        LabelAnchor side = LabelAnchor::TopLeft;
    
    public:
        CCPoint offset;
        CCLabelBMFont* labelNode = nullptr;
        std::string format;
        int presetType = -1;

        LabelModule(std::string format, std::string font);

        void setFont(std::string newFont);
        std::string getFont();

        void setScale(float newScale);
        float getScale();

        void setOpacity(float newOpacity);
        float getOpacity();

        void setSide(LabelAnchor newSide);
        LabelAnchor getSide();

        matjson::Object saveToObject();
        static LabelModule* createFromObject(matjson::Object obj);
};