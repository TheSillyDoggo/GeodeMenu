#include "ExtensionManager.hpp"

ExtensionManager* ExtensionManager::get()
{
    static ExtensionManager* instance = nullptr;

    if (!instance)
        instance = new ExtensionManager();
    
    return instance;
}

void ExtensionManager::addCategory(ext::CategoryData cat)
{
    categories.push_back(cat);
}

std::vector<ext::CategoryData>& ExtensionManager::getCategories()
{
    return categories;
}
