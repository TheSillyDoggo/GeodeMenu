#pragma once

#include "../../include/CategoryExt.hpp"
#include "Module.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace qolmod;

class ExtensionManager
{
    protected:
        std::vector<ext::CategoryData> categories = {};

    public:
        static ExtensionManager* get();

        void addCategory(ext::CategoryData cat);
        std::vector<ext::CategoryData>& getCategories();
};