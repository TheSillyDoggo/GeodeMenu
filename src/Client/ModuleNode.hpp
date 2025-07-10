#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"

using namespace geode::prelude;

class ModuleNode : public CCNode
{
    protected:
        Module* module = nullptr;
    
    public:
        static ModuleNode* create(Module* module);

        virtual void setup();

        void onToggle(CCObject* sender);

        bool init(Module* module);
};