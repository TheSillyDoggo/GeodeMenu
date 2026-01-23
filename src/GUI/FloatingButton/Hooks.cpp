#include "Hooks.hpp"
#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"

bool FloatingMenuLayer::init()
{
    return MenuLayer::init();

    FloatingUIManager::get()->updateSprites();

    MenuLayer::init();
    
    #ifdef GEODE_IS_MOBILE

    if (Loader::get()->getLoadedMod("geode.texture-loader"))
    {
        if (FloatingUIManager::get()->buttons.size() > 0)
        {
            static bool shownWarning = false;

            if (!shownWarning)
            {
                shownWarning = true;

                auto alert = FLAlertLayer::create("QOLMod", "Shortcuts are currently not supported with textureldr installed\nsorry :c", "OK");
                alert->m_scene = this;
                alert->show();
            }
        }
    }

    #endif

    return true;
}

void FloatingTouchDispatcher::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
    #ifdef GEODE_IS_MOBILE
    if (!Loader::get()->getLoadedMod("geode.texture-loader"))
    {
        if (FloatingUIManager::get()->touches(pTouches, pEvent, uIndex))
            return;
    }
    #else
    if (FloatingUIManager::get()->touches(pTouches, pEvent, uIndex))
        return;
    #endif

    CCTouchDispatcher::touches(pTouches, pEvent, uIndex);
}