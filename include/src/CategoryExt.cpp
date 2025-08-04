#include "../CategoryExt.hpp"
#include "../../src/Client/ExtensionManager.hpp"

using namespace geode::prelude;
using namespace QOLMod;

$execute
{
    new EventListener<EventFilter<AddCategoryEvent>>(+[](AddCategoryEvent* ev)
    {
        auto cat = ev->category;

        ExtensionManager::get()->addCategory(cat);

        return ListenerResult::Stop;
    });
}