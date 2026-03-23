#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../IconicPlayerHook.hpp"

using namespace geode::prelude;

class $modify (IconicPlayerObject, PlayerObject)
{
    void startDashing(DashRingObject* object)
    {
        PlayerObject::startDashing(object);

        if (m_dashFireSprite)
        {
            if (auto iconic = typeinfo_cast<IconicPlayerHook*>(getUserObject("iconic-hook"_spr)))
            {
                iconic->onBeginDash(m_dashFireSprite);
            }
        }
    }

    void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to)
    {
        PlayerObject::playSpiderDashEffect(from, to);

        if (auto gjbgl = GJBaseGameLayer::get())
        {
            if (auto spr = gjbgl->m_objectLayer->getChildByType<CCSprite>(-1))
            {
                if (spr->isFrameDisplayed(CCSpriteFrameCache::get()->spriteFrameByName("spiderDash_002.png")))
                {
                    if (auto iconic = typeinfo_cast<IconicPlayerHook*>(getUserObject("iconic-hook"_spr)))
                    {
                        iconic->onSpiderTeleport(spr);
                    }
                }
            }
        }
    }
};