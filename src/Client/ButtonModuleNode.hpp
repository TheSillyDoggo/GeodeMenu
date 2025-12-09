#pragma once

#include "ModuleNode.hpp"
#include "ButtonModule.hpp"
#include "../GUI/BetterSlider.hpp"

class ButtonModuleNode : public ModuleNode
{    
    public:
        static ButtonModuleNode* create(ButtonModule* module);

        void onButtonClicked(CCObject* sender);

        virtual void setup();
};