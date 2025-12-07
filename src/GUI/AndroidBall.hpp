#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AndroidBall : public CCNodeRGBA
{
    protected:
        CCSprite* background = nullptr;
        CCSprite* overlay = nullptr;
        bool colonThreeEnabled = false;
        bool isDragging = false;
        bool isMoving = false;
        CCPoint moveOffset = ccp(0, 0);
        CCPoint position = CCPointZero;

        float scale = 1.0f;
        float normalOpacity = 0.4f;
        bool smoothMove = true;
        
    public:
        CREATE_FUNC(AndroidBall);
        static AndroidBall* get();

        void setColonThreeSecret(bool enabled);
        bool getColonThreeSecret();
        bool shouldFunction();
        void reloadTextures();
        void onOpenMenu();

        void setButtonScale(float scale);
        void setSmoothMove(bool smooth);

        bool ccTouchBegan(CCTouch* touch);
        bool ccTouchMoved(CCTouch* touch);
        bool ccTouchEnded(CCTouch* touch);

        virtual bool init();
        virtual void setOpacity(GLubyte opacity);
        virtual void update(float dt);
        virtual void visit();
};