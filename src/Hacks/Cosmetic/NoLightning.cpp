#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GJBaseGameLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    void lightningFlash(cocos2d::CCPoint from, cocos2d::CCPoint to, cocos2d::ccColor3B color, float lineWidth, float duration, int displacement, bool flash, float opacity)
    {

    }

    QOLMOD_MOD_ALL_HOOKS("no-lightning")
};