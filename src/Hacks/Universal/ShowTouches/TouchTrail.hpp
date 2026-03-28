#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class TouchTrail : public cocos2d::CCNode
    {
        protected:
            cocos2d::CCMotionStreak* streak;
            cocos2d::CCSprite* circle;
            cocos2d::CCLabelBMFont* durationLabel = nullptr;
            cocos2d::CCPoint position = ccp(0, 0);
            float duration = 0;

        public:
            CREATE_FUNC(TouchTrail)

            virtual void setPosition(const cocos2d::CCPoint &position);
            virtual bool init();
            virtual void update(float dt);
    };
}