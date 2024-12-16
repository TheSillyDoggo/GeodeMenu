#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum class LockedAxis
{
    Any,
    Horizontal,
    Vertical,
};

class GrabNodeLayer : public CCLayer
{
    private:
        bool isDragging = false;
        CCNode* nodeToGrab = nullptr;
        CCPoint startPos = CCPointZero;
        std::function<void()> onStartDrag = nullptr;
        std::function<void()> onEndDrag = nullptr;
        LockedAxis axis = LockedAxis::Any;

        CCPoint getAxisForLocked(LockedAxis axis);

    public:
        virtual bool init();

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

        virtual void registerWithTouchDispatcher(void);

        void setNodeToGrab(CCNode* node);
        CCNode* getNodeToGrab();

        void setLockedAxis(LockedAxis axis);
        LockedAxis getLockedAxis();

        void setOnStartDrag(std::function<void()> callback);
        void setOnEndDrag(std::function<void()> callback);

        CREATE_FUNC(GrabNodeLayer);
};