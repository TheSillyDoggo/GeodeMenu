#pragma once

#include "../../include/CategoryExt.hpp"
#include "Module.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace QOLMod;

class ExtensionManager
{
    protected:
        std::vector<CategoryExt> categories = {};
        std::vector<Module*> modules = {};

    public:
        static ExtensionManager* get();

        void addCategory(CategoryExt cat);
        std::vector<CategoryExt> getCategories();

        void addModule(Module* module);
        std::vector<Module*> getModules();
};