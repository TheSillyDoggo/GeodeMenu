#include "../../Client/Module.hpp"
#include "../Utils/PlayLayer.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class AutoCollectCoins : public Module
{
    public:
        MODULE_SETUP(AutoCollectCoins)
        {
            setName("Auto collect coins");
            setID("auto-coins");
            setCategory("Level");
            setDescription("Automatically collect coins in a level");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

SUBMIT_HACK(AutoCollectCoins);

/*class $modify (PlayLayer)
{
    void resetLevel()
    {
        PlayLayer::resetLevel();

        if (AutoCollectCoins::get()->getRealEnabled())
        {
            for (auto coin : PlayLayerUtils::getUtils()->m_fields->coins)
            {
                pickupItem(coin);
                destroyObject(coin);
            }
        }
    }
};*/