#pragma once

#include "ModuleNode.hpp"

class EnumModule;

class EnumModuleNode : public ModuleNode
{    
    protected:
        AdvLabelBMFont* label = nullptr;
        CCMenuItemSpriteExtra* leftBtn = nullptr;
        CCMenuItemSpriteExtra* rightBtn = nullptr;

    public:
        static EnumModuleNode* create(EnumModule* module);

        void onLeft(CCObject* sender);
        void onRight(CCObject* sender);

        void updateButtonEnabled(CCMenuItemSpriteExtra* btn, bool enabled);

        virtual void setup();
        virtual void updateNode();
};