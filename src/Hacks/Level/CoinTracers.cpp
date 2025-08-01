#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include "../Utils/PlayLayer.hpp"

using namespace geode::prelude;

class CoinTracers : public Module
{
    public:
        MODULE_SETUP(CoinTracers)
        {
            setName("Coin Tracers");
            setID("coin-tracers");
            setCategory("Level");
            setDescription("Draws a line to secret / user coins in the level");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

SUBMIT_HACK(CoinTracers);

class $modify (PlayLayer)
{
    virtual void postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        if (CoinTracers::get()->getRealEnabled())
        {
            auto utils = PlayLayerUtils::getUtils()->m_fields.self();

            for (auto coin : utils->coins)
            {
                if (coin->getOpacity() != 0)
                    utils->drawNode->drawSegment(m_player1->getPosition(), coin->getPosition(), 1, ccc4f(1, 0, 0, 1));
            }
        }
    }
};