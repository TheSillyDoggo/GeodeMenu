#pragma once

#include <Touch.hpp>

namespace cocos2d
{
    class CCNode;
};

namespace qolmod
{
    class TouchTrail;

    class ShowTouchLayer : public cocos2d::CCNode
    {
        protected:
            std::unordered_map<int, qolmod::TouchTrail*> trails = {};

        public:
            static ShowTouchLayer* get();

            void touchBegan(qolmod::Touch* touch);
            void touchMoved(qolmod::Touch* touch);
            void touchEnded(qolmod::Touch* touch);
            void touchCancelled(qolmod::Touch* touch);

            virtual void visit();
    };
};