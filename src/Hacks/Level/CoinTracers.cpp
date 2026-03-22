#include "../../Client/Module.hpp"
#include "../../Client/ColourModule.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <BaseDrawNode.hpp>
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

class CoinTracersColour : public ColourModule
{
    public:
        MODULE_SETUP(CoinTracersColour)
        {
            setName("Colour:");
            setID("coin-tracers/colour");
            setDefaultConfig({ ccc3(255, 0, 0)} );
        }
};

SUBMIT_HACK(CoinTracers);
SUBMIT_OPTION(CoinTracers, CoinTracersColour);

namespace qolmod
{
    class CoinTracerNode : public qolmod::BaseDrawNode
    {
        public:
            CREATE_FUNC(CoinTracerNode);

            virtual void redraw()
            {
                clear();

                if (CoinTracers::get()->getRealEnabled())
                {
                    auto utils = PlayLayerUtils::getUtils()->m_fields.self();
                    auto col = CoinTracersColour::get()->getColour();

                    for (auto coin : utils->coins)
                    {
                        if (!coin->hasBeenActivated())
                            drawSegment(gjbgl->m_player1->getPosition(), coin->getPosition(), 1, ccc4f(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1));
                    }
                }
            }
    };
}

class $modify (PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        this->addChild(qolmod::CoinTracerNode::create(), 10);

        return true;
    }
};