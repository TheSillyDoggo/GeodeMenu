#define GEODE_DEFINE_EVENT_EXPORTS
#include "../CategoryExt.hpp"
#include "../../src/Client/ExtensionManager.hpp"

using namespace geode::prelude;

void qolmod::ext::addCustomCategory(CategoryData data)
{
    ExtensionManager::get()->addCategory(data);
}