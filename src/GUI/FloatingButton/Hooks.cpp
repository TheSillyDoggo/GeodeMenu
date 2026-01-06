#include "Hooks.hpp"
#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"

bool FloatingMenuLayer::init()
{
    FloatingUIManager::get()->updateSprites();

    MenuLayer::init();
    
    #ifdef GEODE_IS_MOBILE

    if (Loader::get()->getLoadedMod("geode.texture-loader"))
    {
        if (FloatingUIManager::get()->buttons.size() > 0)
        {
            auto alert = FLAlertLayer::create("Shortcuts", "Shortcuts are currently not supported with textureldr installed\nsorry :c", "OK");
            alert->m_scene = this;
            alert->show();
        }
    }

    #endif

    return true;
}

void FloatingTouchDispatcher::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
    if (FloatingUIManager::get()->touches(pTouches, pEvent, uIndex))
        return;

    CCTouchDispatcher::touches(pTouches, pEvent, uIndex);
}