#include "../../Client/Module.hpp"
#include <Geode/modify/SecretRewardsLayer.hpp>
#include <Geode/modify/GameStatsManager.hpp>

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
            setEnableWarning("Opening lots of chests with this cheat may lead to you getting <cr>leaderboard banned</c>.\nIt is not my (the developers) fault if you get banned from opening every chest.\nYou can undo every illegitimately obtained chest by disabling <cc>Treasure Room Chest Bypass</c> and <cl>restarting the game</c>");
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
        GameStatsManager::sharedState()->setStat("21", 999999);

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
        GameStatsManager::sharedState()->setStat("43", 999999);

        SecretRewardsLayer::onGoldChest(sender);

        GameStatsManager::sharedState()->setStat("21", keys);

        if (m_goldKeysLabel)
            m_goldKeysLabel->setString(fmt::format("{}", keys).c_str());
    }
};

class $modify (GameStatsManager)
{
    void recountSpecialStats()
    {
        if (!OpenChestBypass::get()->getRealEnabled())
            GameStatsManager::recountSpecialStats();
    }
};