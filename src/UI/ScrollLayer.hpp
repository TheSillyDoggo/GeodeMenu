#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class ScrollLayer : public geode::ScrollLayer
    {
        protected:
            ScrollLayer(cocos2d::CCRect const& rect, bool scrollWheelEnabled, bool vertical);

        public:
            static ScrollLayer* create(cocos2d::CCSize const& size, bool scrollWheelEnabled = true, bool vertical = true);
    };
};