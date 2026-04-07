#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class AltMouseDelegate
    {
        public:
            virtual bool altClickBegan(int button, cocos2d::CCPoint point) { return false; }
            virtual void altClickMoved(int button, cocos2d::CCPoint point) {}
            virtual void altClickEnded(int button, cocos2d::CCPoint point) {}
    };
};