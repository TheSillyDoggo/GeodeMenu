#pragma once

#include <Geode/Geode.hpp>

class CheatIndicator : public cocos2d::CCNode
{
    protected:
        cocos2d::CCSprite* spr;

    public:
        CREATE_FUNC(CheatIndicator);

        virtual bool init();
        virtual void update(float dt);
};