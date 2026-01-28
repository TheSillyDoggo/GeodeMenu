#pragma once

#include "Module.hpp"

class ButtonModule : public Module
{
    public:
        virtual void onClick() {}
        
        virtual void onKeybindActivated(KeyState state);
        virtual ModuleNode* getNode();
        virtual std::string getNotificationString();
};