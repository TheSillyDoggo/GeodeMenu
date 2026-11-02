#define GEODE_DEFINE_EVENT_EXPORTS
#include <Geode/Geode.hpp>
#include "../../api/AntiCheat.hpp"
#include <APIManager.hpp>

using namespace geode::prelude;
using namespace qolmod;

void qolmod::api::enableAntiCheat(Mod* mod)
{
    APIManager::get()->addAntiCheatMod(mod);
}

void qolmod::api::disableAntiCheat(Mod* mod)
{
    APIManager::get()->removeAntiCheatMod(mod);
}