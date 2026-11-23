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



#include <Geode/modify/MenuLayer.hpp>

class $modify (MenuLayer)
{
    void onNewgrounds(cocos2d::CCObject* sender)
    {
        qolmod::api::enableAntiCheat();
        qolmod::api::enableAntiCheat(Loader::get()->getInstalledMod("capeling.soggy-mod"));
        qolmod::api::enableAntiCheat(Loader::get()->getInstalledMod("alphalaneous.alphas_geode_utils"));
    }

    void onNews(cocos2d::CCObject* sender)
    {
        qolmod::api::disableAntiCheat();
        qolmod::api::disableAntiCheat(Loader::get()->getInstalledMod("capeling.soggy-mod"));
        qolmod::api::disableAntiCheat(Loader::get()->getInstalledMod("alphalaneous.alphas_geode_utils"));
    }
};