#include "ExtensionManager.hpp"

ExtensionManager* ExtensionManager::get()
{
    static ExtensionManager* instance = nullptr;

    if (!instance)
        instance = new ExtensionManager();
    
    return instance;
}

void ExtensionManager::addCategory(CategoryExt cat)
{
    categories.push_back(cat);
}

std::vector<CategoryExt> ExtensionManager::getCategories()
{
    return categories;
}

void ExtensionManager::addModule(Module* module)
{
    modules.push_back(module);
}

std::vector<Module*> ExtensionManager::getModules()
{
    return modules;
}