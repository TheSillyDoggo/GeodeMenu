#include "Hooks.hpp"
#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"

$on_game(Loaded)
{
    FloatingUIManager::get()->updateSprites();
}

void FloatingTouchDispatcher::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
    if (FloatingUIManager::get()->touches(pTouches, pEvent, uIndex))
        return;

    CCTouchDispatcher::touches(pTouches, pEvent, uIndex);
}

void FloatingTouchDispatcher::onModify(auto& self)
{
    #ifdef GEODE_IS_MOBILE
    (void)self.setHookPriorityPre("cocos2d::CCTouchDispatcher::touches", -8008135);
    #endif
}