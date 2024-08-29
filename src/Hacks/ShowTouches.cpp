#include <Geode/Geode.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCTouchDispatcher)
{
    void touches(CCSet* touches, CCEvent* event, unsigned int type)
    {
        CCTouchDispatcher::touches(touches, event, type);

        if (!CCScene::get())
            return;

        if (getChildOfType<LoadingLayer>(CCScene::get(), 0))
            return;

        if (type == CCTOUCHBEGAN)
        {
            if (auto touch = as<CCTouch*>(touches->anyObject()))
            {
                if (CCScene::get(); auto circle = CCCircleWave::create(0, 20, 0.2f, true))
                {    
                    circle->m_opacity = 0.8f;
                    circle->m_lineWidth = 3;
                    circle->m_blendAdditive = true;
                    circle->setPosition(touch->getLocation());
                    
                    CCScene::get()->addChild(circle, CCScene::get()->getHighestChildZ() + 1);
                }
            }
        }
    }

    QOLMOD_MOD_HOOK("show-touches", "cocos2d::CCTouchDispatcher::touches")
};