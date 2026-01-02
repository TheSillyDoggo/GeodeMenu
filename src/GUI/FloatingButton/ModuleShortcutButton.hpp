#pragma once

#include "FloatingUIButton.hpp"
#include "../../Client/Module.hpp"

class ModuleShortcutButton : public FloatingUIButton
{
    protected:
        Module* mod = nullptr;
        bool lastUpdated = false;
        std::string overlaySprite = "";

    public:
        static ModuleShortcutButton* create(Module* module);

        void setup();
        void updateSprs();
        void setOverlaySprite(std::string spr);

        virtual void updatePosition(cocos2d::CCPoint point);
        virtual void update(float dt);
};