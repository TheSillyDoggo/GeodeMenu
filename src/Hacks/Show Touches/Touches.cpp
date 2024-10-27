#include <Geode/Geode.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include "../../Client/Client.h"
#include "TouchTrail.hpp"

using namespace geode::prelude;

class $modify (CCTouchDispatcher)
{
    void touches(CCSet* touches, CCEvent* event, unsigned int type)
    {
        if (!CCScene::get())
            return CCTouchDispatcher::touches(touches, event, type);;

        if (getChildOfType<LoadingLayer>(CCScene::get(), 0))
            return CCTouchDispatcher::touches(touches, event, type);;

        if (auto touch = as<CCTouch*>(touches->anyObject()))
        {
            if (type == CCTOUCHBEGAN)
            {
                CCScene::get()->addChild(CCTouchTrail::create(touch), CCScene::get()->getHighestChildZ() + 1);
            }

            if (type >= CCTOUCHENDED)
            {
                CCTouchTrail::remove(touch);
            }
        }

        CCTouchDispatcher::touches(touches, event, type);
    }

    QOLMOD_MOD_HOOK("show-touches", "cocos2d::CCTouchDispatcher::touches")
};

class $modify (CCMotionStreak)
{
    static CCMotionStreak* create(float fade, float minSeg, float stroke, ccColor3B const& color, char const* path)
    {
        log::info("fade: {}, minSeg: {}, stroke: {}", fade, minSeg, stroke);

        return CCMotionStreak::create(fade, minSeg, stroke, color, path);
    }
};