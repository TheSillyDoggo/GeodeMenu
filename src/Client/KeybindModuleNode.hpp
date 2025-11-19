#pragma once

#include "ModuleNode.hpp"
#include "KeybindModule.hpp"
#include "../GUI/KeycodeNode.hpp"

class KeybindModuleNode : public ModuleNode
{
    protected:
        KeycodeNode* keycodeNode = nullptr;

    public:
        static KeybindModuleNode* create(KeybindModule* module);

        void onChangeBind(CCObject* sender);

        virtual void setup();
        virtual void updateNode();
};