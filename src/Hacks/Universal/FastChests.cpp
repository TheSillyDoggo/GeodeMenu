#include "../../Client/Module.hpp"
#include <Geode/modify/RewardUnlockLayer.hpp>

using namespace geode::prelude;

class FastChests : public Module
{
    public:
        MODULE_SETUP(FastChests)
        {
            setID("fast-chests");
            setCategory("Universal");
        }
};

SUBMIT_HACK(FastChests);

class $modify (RewardUnlockLayer)
{
    bool showCollectReward(GJRewardItem* item)
    {
        auto ret = RewardUnlockLayer::showCollectReward(item);

        if (FastChests::get()->getRealEnabled())
            showCloseButton();

        return true;
    }
};