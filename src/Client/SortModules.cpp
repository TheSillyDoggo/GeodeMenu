#include <Geode/Geode.hpp>
#include "Module.hpp"

using namespace geode::prelude;

$execute
{
    Loader::get()->queueInMainThread([]
    {
        Module::sortAlphabetically();
    });
}

void Module::sortAlphabetically()
{
    std::sort(Module::moduleMap.begin(), Module::moduleMap.end(), [](Module* a, Module* b)
    {
        return a->getName() < b->getName();
    });
}