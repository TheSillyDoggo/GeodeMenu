#pragma once

#include "ModuleNode.hpp"
#include "InputModule.hpp"

class InputModuleNode : public ModuleNode, public TextInputDelegate
{
    protected:
        ~InputModuleNode();
        TextInput* input = nullptr;
    
    public:
        static InputModuleNode* create(InputModule* module);

        virtual void setup();
        virtual void textChanged(CCTextInputNode* p0);
        virtual void update(float dt);
        virtual void updateNode();
};