#pragma once

#include "Module.hpp"

class ButtonModule : public Module
{
    public:
        virtual void onClick() {}

        bool shouldSave();
        
        virtual ModuleNode* getNode();
};