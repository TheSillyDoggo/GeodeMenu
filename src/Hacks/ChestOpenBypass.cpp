#include <Geode/Geode.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (SecretRewardsLayer)
{
    void onSelectItem(cocos2d::CCObject* sender)
    {
        auto keys = GameStatsManager::sharedState()->getStat("21");
        GameStatsManager::sharedState()->setStat("21", 9999);

        SecretRewardsLayer::onSelectItem(sender);

        GameStatsManager::sharedState()->setStat("21", keys);

        m_keysLabel->setString(fmt::format("{}", keys).c_str());
    }

    QOLMOD_MOD_HOOK("chest-unlock-bypass", "SecretRewardsLayer::onSelectItem")
};