#include "../../Client/Module.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class NoLightning : public Module
{
    public:
        MODULE_SETUP(NoLightning)
        {
            setName("No Lightning");
            setID("no-lightning");
            setCategory("Cosmetic");
            setDescription("Disables the lightning when touching certain portals");
        }
};

SUBMIT_HACK(NoLightning);

class $modify (GJBaseGameLayer)
{
    void lightningFlash(cocos2d::CCPoint from, cocos2d::CCPoint to, cocos2d::ccColor3B color, float lineWidth, float duration, int displacement, bool flash, float opacity)
    {
        if (NoLightning::get()->getRealEnabled())
            return;

        GJBaseGameLayer::lightningFlash(from, to, color, lineWidth, duration, displacement, flash, opacity);
    }
};