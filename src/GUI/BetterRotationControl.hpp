#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BetterRotationControl : public CCMenu
{
    protected:
        GJRotationControlDelegate* delegate;
        CCSprite* handle = nullptr;
        float lastAngle = 0;

        virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
        virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
        virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

    public:
        CREATE_FUNC(BetterRotationControl);

        void setAngle(float angle);
        void setDelegate(GJRotationControlDelegate* delegate);

        virtual bool init();
        virtual void draw();
};