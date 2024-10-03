#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RepeatableMenuItemSpriteExtra : public CCMenuItemSpriteExtra
{
    public:
        float maxSpeed = 0.03f;
        float repTimeInc = 0.003f;
        float repeatTime = 0.15f;
        float startDelay = 0.4f;
        float t = 0;
        float v = 0;
        float d = 0;

        virtual void update(float dt);

        static RepeatableMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCNode* disabledSprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
        static RepeatableMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
};