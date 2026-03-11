#pragma once

#include "../../Client/Module.hpp"
#include "ZoomControl.hpp"

using namespace geode::prelude;

class ZoomInterceptNode : public CCLayer
{
    protected:
        float zoom = 0;
        CCPoint zoomedPos = ccp(0, 0);
        bool isPanning = false;
        CCLayer* layer = nullptr;
        CCTouch* touch1 = nullptr;
        CCTouch* touch2 = nullptr;
        float lastPinchDis = 0;
        qolmod::ZoomControl* control = nullptr;
        static inline ZoomInterceptNode* instance = nullptr;

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

        virtual void scrollWheel(float y, float x);
        virtual void registerWithTouchDispatcher();
        virtual void onExit();
        ~ZoomInterceptNode();

    public:
        static ZoomInterceptNode* create(CCLayer* layer);
        static ZoomInterceptNode* get();

        void setPreviewPos(CCPoint pos);
        void setZoom(float zool);
        void updatePositions();
        bool isZooming();

        CCPoint getPreviewPos();
        float getZoom();
        
        virtual void update(float dt);
        virtual bool init();
};