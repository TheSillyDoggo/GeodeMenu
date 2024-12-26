#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/SecretRewardsLayer.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GameStatsManager.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (SecretRewardsLayer)
{
    void onSelectItem(cocos2d::CCObject* sender)
    {
        auto keys = GameStatsManager::sharedState()->getStat("21");
        GameStatsManager::sharedState()->setStat("21", 9999);

        SecretRewardsLayer::onSelectItem(sender);

        GameStatsManager::sharedState()->setStat("21", keys);

        if (m_keysLabel)
            m_keysLabel->setString(fmt::format("{}", keys).c_str());
    }

    void onGoldChest(cocos2d::CCObject* sender)
    {
        auto keys = GameStatsManager::sharedState()->getStat("43");
        GameStatsManager::sharedState()->setStat("43", 9999);

        SecretRewardsLayer::onGoldChest(sender);

        GameStatsManager::sharedState()->setStat("21", keys);

        if (m_goldKeysLabel)
            m_goldKeysLabel->setString(fmt::format("{}", keys).c_str());
    }

    QOLMOD_MOD_ALL_HOOKS("chest-unlock-bypass")
};