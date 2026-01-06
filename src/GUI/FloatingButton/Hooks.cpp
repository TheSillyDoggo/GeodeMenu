#include "Hooks.hpp"
#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"

bool FloatingMenuLayer::init()
{
    FloatingUIManager::get()->updateSprites();

    return MenuLayer::init();
}

void FloatingTouchDispatcher::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
    if (FloatingUIManager::get()->touches(pTouches, pEvent, uIndex))
        return;

    CCTouchDispatcher::touches(pTouches, pEvent, uIndex);
}