#pragma once

#include "FloatingUIButton.hpp"
#include "../../Client/Module.hpp"

class ModuleShortcutButton : public FloatingUIButton
{
    protected:
        Module* mod = nullptr;
        bool lastUpdated = false;
        std::string overlaySprite = "";
        std::string bgOffSpr = "geode.loader/baseCircle_Medium_Gray.png";
        std::string bgOnSpr = "geode.loader/baseCircle_Medium_Green.png";

    public:
        static ModuleShortcutButton* create(Module* module);

        void setup();
        void updateSprs();
        void setOverlaySprite(std::string spr);
        void setBackgroundSprites(std::string bgOff, std::string bgOn);

        virtual void updatePosition(cocos2d::CCPoint point);
        virtual void update(float dt);
};