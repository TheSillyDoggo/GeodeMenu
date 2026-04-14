#pragma once

#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/touch_dispatcher/CCTouch.h>

namespace qolmod
{
    struct Touch
    {
        cocos2d::CCPoint startLocation;
        cocos2d::CCPoint location;
        int id;
        cocos2d::CCTouch* fakeTouch;

        void updateFakeTouch();

        Touch() = default;
        Touch(int id, cocos2d::CCPoint point);
        ~Touch();
    };
};