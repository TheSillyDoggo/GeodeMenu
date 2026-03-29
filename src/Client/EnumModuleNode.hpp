#pragma once

#include "ModuleNode.hpp"
#include <Button.hpp>

class EnumModule;

class EnumModuleNode : public ModuleNode
{    
    protected:
        AdvLabelBMFont* label = nullptr;
        qolmod::Button* leftBtn = nullptr;
        qolmod::Button* rightBtn = nullptr;

    public:
        static EnumModuleNode* create(EnumModule* module);

        void onLeft(CCObject* sender);
        void onRight(CCObject* sender);

        void updateButtonEnabled(qolmod::Button* btn, bool enabled);

        virtual void setup();
        virtual void updateNode();
};