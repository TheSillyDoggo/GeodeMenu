#pragma once

#include "Module.hpp"

class ButtonModule : public Module
{
    public:
        virtual void onClick() {}

        bool shouldSave();
        
        virtual void onKeybindActivated(KeyState state);
        virtual ModuleNode* getNode();
};