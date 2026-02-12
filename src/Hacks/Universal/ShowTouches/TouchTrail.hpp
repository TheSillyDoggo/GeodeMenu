#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CCTouchTrail : public CCNode
{
    public:
        static inline std::unordered_map<CCTouch*, CCTouchTrail*> touches = {};
        CCTouch* touch;
        CCMotionStreak* streak;
        CCSprite* circle;
        CCLabelBMFont* durationLabel = nullptr;
        float duration = 0;

        bool init(CCTouch* touch);

        virtual void update(float dt);

        static CCTouchTrail* create(CCTouch* touch);
        static void remove(CCTouch* touch);
};