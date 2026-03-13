#include "../../Client/Module.hpp"
#include <Geode/modify/CCCircleWave.hpp>

using namespace geode::prelude;

class HighQualityWave : public Module
{
    public:
        MODULE_SETUP(HighQualityWave)
        {
            setID("high-quality-circles");
            setCategory("Cosmetic");
        }
};

SUBMIT_HACK(HighQualityWave);

void myDrawCircle(const cocos2d::CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter)
{
    if (HighQualityWave::get()->getRealEnabled())
        segments *= 5;
    
    cocos2d::ccDrawCircle(center, radius, angle, segments, drawLineToCenter);
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(
                geode::modifier::Resolve<const cocos2d::CCPoint&, float, float, unsigned int, bool>::func(&cocos2d::ccDrawCircle)
            )
        ),
        &myDrawCircle,
        "cocos2d::ccDrawCircle",
        tulip::hook::TulipConvention::Cdecl
    );
}