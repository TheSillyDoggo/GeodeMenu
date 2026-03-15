#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class NoRenderNode : public cocos2d::CCNode
    {
        public:
            CREATE_FUNC(NoRenderNode)

            virtual void visit();
    };
};