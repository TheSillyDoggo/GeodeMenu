#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BetterScrollbar : public CCMenu
{
    protected:
        CCScrollLayerExt* layer = nullptr;
        CCScale9Sprite* background = nullptr;
        CCNode* handleContainer = nullptr;
        CCScale9Sprite* handle = nullptr;
        bool scrollbarSelected = false;
        float inset = 1.6f;
        float handleHeight = 18;
        bool dynamicHandle = false;
        bool disabled = false;

        float scrollMaxUp = 0;
        float scrollMaxDown = 0;
        float scrollStart = 0;

    public:
        CREATE_FUNC(BetterScrollbar)
        static BetterScrollbar* create(CCScrollLayerExt* layer);

        void setLayer(CCScrollLayerExt* layer);
        CCScrollLayerExt* getLayer();

        void setInset(float inset);
        float getInset();

        void setHandleHeight(float height);
        float getHandleHeight();

        void setUseDynamicHandle(bool dynamic);
        bool getUseDynamicHandle();

        void setDisabled(bool disabled);
        bool isDisabled();

        CCScale9Sprite* getBackground();

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

        virtual bool init();
        virtual void visit();
};