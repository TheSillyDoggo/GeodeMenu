#include "Hooks.hpp"
#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"

void FloatingEGLView::swapBuffers()
{
    if (!AndroidUI::get())
        FloatingUIManager::get()->visit();

    CCEGLView::swapBuffers();
}

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