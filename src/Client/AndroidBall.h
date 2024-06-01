#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>
#include <Geode/modify/CCScene.hpp>
#include "Client.h"
#include "AndroidUI.h"

class AndroidBall : public CCLayer
{
    public:
        static inline bool hasPos = false;
        static inline CCPoint position = ccp(32, CCDirector::get()->getWinSize().height / 2);
        static inline AndroidBall* instance = nullptr;
        static inline int highest = 0;

        bool doingThing = false;
        static inline bool dragging = false;
        CircleButtonSprite* btn;
        CCLabelBMFont* l;
        CCMenu* menu;

        Module* mod = nullptr;
        Module* mod2 = nullptr;
        Module* canDrag = nullptr;

        void onOpenMenu();

        virtual bool init();

        virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

        virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

        virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

        virtual void update(float dt);

        static float clampf(float v, float min, float max);

        void UpdateVisible(bool i);

        CREATE_FUNC(AndroidBall);

        ~AndroidBall();
};