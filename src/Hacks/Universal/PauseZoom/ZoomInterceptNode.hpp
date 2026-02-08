#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class ZoomInterceptNode : public CCLayer
{
    protected:
        ~ZoomInterceptNode();

    public:
        float zoom = 0;
        CCPoint zoomedPos = ccp(0, 0);

        CREATE_FUNC(ZoomInterceptNode);

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
        virtual void scrollWheel(float y, float x);
        
        virtual void update(float dt);
        virtual bool init();
};