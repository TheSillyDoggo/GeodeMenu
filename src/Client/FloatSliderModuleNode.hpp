#pragma once

#include "ModuleNode.hpp"
#include "FloatSliderModule.hpp"
#include "../GUI/BetterSlider.hpp"

class FloatSliderModuleNode : public ModuleNode
{
    protected:
        BetterSlider* slider = nullptr;
    
    public:
        static FloatSliderModuleNode* create(FloatSliderModule* module);

        void onSliderMoved(CCObject* sender);

        virtual void setup();
        virtual void updateNode();
};