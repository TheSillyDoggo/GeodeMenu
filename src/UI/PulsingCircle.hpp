#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class PulsingCircle : public cocos2d::CCNode, public CCCircleWaveDelegate
    {
        public:
            float startRadius;
            float endRadius;
            float duration;
            bool fadeIn;
            bool easeOut;
            cocos2d::ccColor3B color = cocos2d::ccWHITE;
            CircleMode circleMode = CircleMode::Filled;
            int lineWidth = 1;
            float opacityMod = 1.0f;
            bool blendAdditive = true;
            
            void addCircle();

        public:
            static PulsingCircle* create(float startRadius, float endRadius, float duration, bool fadeIn, bool easeOut);
            CREATE_FUNC(PulsingCircle);

            virtual void circleWaveWillBeRemoved(CCCircleWave* circleWave);
            virtual void onEnter();
    };
};