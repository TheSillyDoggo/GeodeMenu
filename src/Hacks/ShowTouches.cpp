#include <Geode/Geode.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCTouchDispatcher)
{
    void touches(CCSet* touches, CCEvent* event, unsigned int type)
    {
        CCTouchDispatcher::touches(touches, event, type);

        if (type == CCTOUCHBEGAN)
        {
            if (auto touch = as<CCTouch*>(touches->anyObject()))
            {
                auto circle = CCCircleWave::create(0, 20, 0.2f, true);
                circle->m_opacity = 0.8f;
                circle->m_lineWidth = 3;
                circle->m_blendAdditive = true;
                circle->setPosition(touch->getLocation());
                
                CCScene::get()->addChild(circle, CCScene::get()->getHighestChildZ());
            }
        }
    }

    QOLMOD_MOD_HOOK("show-touches", "cocos2d::CCTouchDispatcher::touches")
};