#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"

using namespace geode::prelude;

class CategoryNode : public CCNode
{
    protected:
        std::vector<Module*> modules = {};
    
    public:
        static CategoryNode* create();

        void addModule(Module* module);

        bool init();
};