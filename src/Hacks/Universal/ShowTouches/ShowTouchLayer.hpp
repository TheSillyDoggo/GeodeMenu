#pragma once

namespace cocos2d
{
    class CCNode;
    class CCPoint;
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

            void touchBegan(int id, cocos2d::CCPoint point);
            void touchMoved(int id, cocos2d::CCPoint point);
            void touchEnded(int id, cocos2d::CCPoint point);
            void touchCancelled(int id, cocos2d::CCPoint point);
    };
};