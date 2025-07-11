#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AndroidBall : public CCNode
{
    protected:
        CCSprite* background = nullptr;
        CCSprite* overlay = nullptr;
        bool colonThreeEnabled = false;
        bool isDragging = false;
        bool isMoving = false;
        CCPoint moveOffset = ccp(0, 0);
        
    public:
        CREATE_FUNC(AndroidBall);
        static AndroidBall* get();

        void setColonThreeSecret(bool enabled);
        bool getColonThreeSecret();
        bool shouldFunction();
        void reloadTextures();
        void onOpenMenu();

        bool ccTouchBegan(CCTouch* touch);
        bool ccTouchMoved(CCTouch* touch);
        bool ccTouchEnded(CCTouch* touch);

        virtual void update(float dt);
        virtual bool init();
        virtual void visit();
};