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

void myDrawCircle(const cocos2d::CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY)
{
    if (HighQualityWave::get()->getRealEnabled())
        segments *= 5;

    #if GEODE_COMP_GD_VERSION == 22074 && defined(GEODE_IS_IOS)
    reinterpret_cast<void(__cdecl*)(const cocos2d::CCPoint&, float, float, unsigned int, bool, float, float)>(geode::base::get() + 0x24b660)(center, radius, angle, segments, drawLineToCenter, scaleX, scaleY);
    #else
    cocos2d::ccDrawCircle(center, radius, angle, segments, drawLineToCenter, scaleX, scaleY);
    #endif
}

$execute
{
    if (Loader::get()->isPatchless())
    {
        #ifdef GEODE_IS_IOS

        #if GEODE_COMP_GD_VERSION == 22081
        #define IOS_CCDRAWCIRCLE_OFFSET 0x24c56c
        #endif

        #if GEODE_COMP_GD_VERSION == 22074
        #define IOS_CCDRAWCIRCLE_OFFSET 0x24b660
        #endif

        (void)GEODE_MOD_STATIC_HOOK(IOS_CCDRAWCIRCLE_OFFSET, &myDrawCircle, "cocos2d::ccDrawCircle");

        #endif
    }
    else
    {
        (void)Mod::get()->hook(
            reinterpret_cast<void*>(
                #if GEODE_COMP_GD_VERSION == 22074 && defined(GEODE_IS_IOS)
                geode::base::get() + 0x24b660
                #else
                geode::addresser::getNonVirtual(
                    geode::modifier::Resolve<const cocos2d::CCPoint&, float, float, unsigned int, bool, float, float>::func(&cocos2d::ccDrawCircle)
                )
                #endif
            ),
            &myDrawCircle,
            "cocos2d::ccDrawCircle",
            tulip::hook::TulipConvention::Cdecl
        );
    }
}

/*$execute {
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
}*/