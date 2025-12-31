#pragma once

#include "ModuleNode.hpp"
#include "RotationModule.hpp"

class RotationModuleNode : public ModuleNode
{
    public:
        static RotationModuleNode* create(RotationModule* module);

        void onChangeRotation(CCObject* sender);

        virtual void setup();
        virtual void updateNode();
};