#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"

using namespace geode::prelude;

class ModuleNode : public CCMenu
{
    protected:
        Module* module = nullptr;
    
    public:
        static ModuleNode* create(Module* module);

        virtual void setup();

        void onToggle(CCObject* sender);
        void onToggleError(CCObject* sender);
        void onInfo(CCObject* sender);

        bool init(Module* module);
};