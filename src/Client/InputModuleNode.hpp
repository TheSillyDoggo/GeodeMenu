#pragma once

#include "ModuleNode.hpp"
#include "InputModule.hpp"
#include "../GUI/InputNode.hpp"

class InputModuleNode : public ModuleNode
{
    protected:
        ~InputModuleNode();
        qolmod::InputNode* input = nullptr;
    
    public:
        static InputModuleNode* create(InputModule* module);

        virtual void setup();
        virtual void textChanged(const std::string& str);
        virtual void update(float dt);
        virtual void updateNode();
};