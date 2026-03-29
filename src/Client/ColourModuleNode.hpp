#pragma once

#include "ModuleNode.hpp"
#include "ColourModule.hpp"
#include "../GUI/BetterSlider.hpp"
#include <Button.hpp>

class ColourModuleNode : public ModuleNode
{
    protected:
        CCSprite* colourSpr;
        qolmod::Button* colourBtn;
    
    public:
        static ColourModuleNode* create(ColourModule* module);
        
        void onChangeColour(CCObject* sender);

        virtual void update(float dt);
        virtual void setup();
        virtual void updateNode();
};