#include <Geode/Geode.hpp>
#include "Module.hpp"

using namespace geode::prelude;

$on_mod(Loaded)
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
        auto aN = utils::string::toLower(a->getName());
        auto bN = utils::string::toLower(b->getName());

        return aN < bN;
    });
}