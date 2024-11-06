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

        if (CCScene::get()->getChildByType<LoadingLayer>(0))
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