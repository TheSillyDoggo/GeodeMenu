#include "../../Client/Module.hpp"
#include <Geode/modify/SecretRewardsLayer.hpp>

using namespace geode::prelude;

class OpenChestBypass : public Module
{
    public:
        MODULE_SETUP(OpenChestBypass)
        {
            setName("Treasure Room Chest Bypass");
            setID("chest-unlock-bypass");
            setCategory("Universal");
            setDescription("Allows you to open any chest in the <cc>treasure room</c>");
        }
};

SUBMIT_HACK(OpenChestBypass)

class $modify (SecretRewardsLayer)
{
    void onSelectItem(cocos2d::CCObject* sender)
    {
        if (!OpenChestBypass::get()->getRealEnabled())
            return SecretRewardsLayer::onSelectItem(sender);

        auto keys = GameStatsManager::sharedState()->getStat("21");
        GameStatsManager::sharedState()->setStat("21", 9999);

        SecretRewardsLayer::onSelectItem(sender);

        GameStatsManager::sharedState()->setStat("21", keys);

        if (m_keysLabel)
            m_keysLabel->setString(fmt::format("{}", keys).c_str());
    }

    void onGoldChest(cocos2d::CCObject* sender)
    {
        if (!OpenChestBypass::get()->getRealEnabled())
            return SecretRewardsLayer::onGoldChest(sender);

        auto keys = GameStatsManager::sharedState()->getStat("43");
        GameStatsManager::sharedState()->setStat("43", 9999);

        SecretRewardsLayer::onGoldChest(sender);

        GameStatsManager::sharedState()->setStat("21", keys);

        if (m_goldKeysLabel)
            m_goldKeysLabel->setString(fmt::format("{}", keys).c_str());
    }
};