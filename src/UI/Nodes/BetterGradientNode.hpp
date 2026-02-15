#pragma once

#include <Geode/Geode.hpp>

struct GradientPoint
{
    float position;
    cocos2d::ccColor4B colour;

    GradientPoint(float position, cocos2d::ccColor4B colour);
};

class BetterGradientNode : public cocos2d::CCNode
{
    protected:

    public:
        CREATE_FUNC(BetterGradientNode);

        virtual bool init();
        virtual void draw();
};