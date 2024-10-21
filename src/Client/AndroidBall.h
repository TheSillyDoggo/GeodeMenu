#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/AchievementNotifier.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include <Geode/modify/CCScene.hpp>
#include "Client.h"
#include "AndroidUI.h"

class AndroidBall : public CCLayer
{
    public:
        static inline bool hasPos = false;
        static inline CCPoint position = ccp(32, CCDirector::get()->getWinSize().height / 2);
        static inline AndroidBall* instance = nullptr;

        bool doingThing = false;
        static inline bool dragging = false;
        CCSprite* btnOverlay;
        CCSprite* btn;
        CCLabelBMFont* l;
        CCMenu* menu;

        Module* mod = nullptr;
        Module* mod2 = nullptr;
        Module* canDrag = nullptr;

        CREATE_FUNC(AndroidBall);
        static AndroidBall* get();

        bool isColonThreeEnabled();
        void setColonThreeEnabled();

        void onOpenMenu();

        virtual bool init();

        bool _ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
        bool _ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
        bool _ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

        virtual void update(float dt);
        static float clampf(float v, float min, float max);

        void UpdateVisible(bool i);

        bool editorShouldBeVisible();

        ~AndroidBall();
};

class $modify (QOLModTouchDispatcher, CCTouchDispatcher)
{
    static void onModify(auto& self)
    {
        self.setHookPriority("cocos2d::CCTouchDispatcher::touches", -99999999);
    }

    void touches(CCSet* touches, CCEvent* event, unsigned int type);
};