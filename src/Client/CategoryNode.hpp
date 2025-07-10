#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"
#include "ModuleNode.hpp"

using namespace geode::prelude;

class CategoryNode : public CCMenu
{
    protected:
        std::map<Module*, ModuleNode*> modules = {};
        ScrollLayer* scroll = nullptr;
        CCMenu* contentMenu = nullptr;
    
    public:
        static CategoryNode* create();

        void addModule(Module* module);

        bool init();
};