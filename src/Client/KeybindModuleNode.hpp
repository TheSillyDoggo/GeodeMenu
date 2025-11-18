#pragma once

#include "ModuleNode.hpp"
#include "KeybindModule.hpp"

class KeybindModuleNode : public ModuleNode
{
    protected:
        CCScale9Sprite* keybindBG = nullptr;
        CCLabelBMFont* keybindLabel = nullptr;    

    public:
        static KeybindModuleNode* create(KeybindModule* module);

        void onChangeBind(CCObject* sender);

        virtual void setup();
        virtual void updateNode();
};