#pragma once

#include <Geode/Geode.hpp>
#include <Touch.hpp>
#include "GestureNode.hpp"

namespace qolmod
{
    class GestureManager : public cocos2d::CCNode
    {
        protected:
            std::unordered_map<int, geode::Ref<GestureNode>> trackedTouches = {};

            virtual void draw();

            std::vector<GestureNode*> getNodes();

        public:
            static GestureManager* get();

            bool touchBegan(qolmod::Touch* touch);
            bool touchMoved(qolmod::Touch* touch);
            bool touchEnded(qolmod::Touch* touch);
            bool touchCancelled(qolmod::Touch* touch);
    };
};