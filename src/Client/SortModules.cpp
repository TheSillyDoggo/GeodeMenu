#include <Geode/Geode.hpp>
#include "Module.hpp"

using namespace geode::prelude;

$on_mod(Loaded)
{
    Loader::get()->queueInMainThread([]
    {
        Module::sortAlphabetically(&Module::getAll());
    });
}

void Module::sortAlphabetically(std::vector<Module*>* map)
{
    std::sort(map->begin(), map->end(), [](Module* a, Module* b)
    {
        auto aP = a->getSortPriority();
        auto bP = b->getSortPriority();

        if (aP == bP)
        {
            auto aN = utils::string::toLower(a->getID());
            auto bN = utils::string::toLower(b->getID());

            return aN < bN;
        }

        return aP < bP;
    });
}