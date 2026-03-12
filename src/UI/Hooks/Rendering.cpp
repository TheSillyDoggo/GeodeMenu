#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <NotificationManager.hpp>
#include <Modules/Modules.hpp>
#include "../../GUI/FloatingButton/FloatingUIManager.hpp"
#include "../../GUI/AndroidUI.hpp"
#include "../../GUI/AndroidBall.hpp"
#include "../../Hacks/Universal/Paint/PaintNode.hpp"

using namespace geode::prelude;
using namespace qolmod;

#ifdef GEODE_IS_ANDROID

class $modify (QOLModRenderingHook, CCDirector)
{
    void drawScene(void)
    {
        CCDirector::drawScene();

        if (!CCScene::get() || CCScene::get()->getChildByType<LoadingLayer>(0))
            return CCDirector::drawScene();

        if (!AndroidUI::get())
        {
            FloatingUIManager::get()->visit();
            AndroidBall::get()->visit();
        }

        if (NotificationsEnabled::get()->getRealEnabled())
            NotificationManager::get()->visit();
    }
};

#else

class $modify (QOLModRenderingHook, CCEGLView)
{
    virtual void swapBuffers()
    {
        if (!CCScene::get() || CCScene::get()->getChildByType<LoadingLayer>(0))
            return CCEGLView::swapBuffers();

        if (!AndroidUI::get())
        {
            FloatingUIManager::get()->visit();
            AndroidBall::get()->visit();
        }

        if (NotificationsEnabled::get()->getRealEnabled())
            NotificationManager::get()->visit();

        CCEGLView::swapBuffers();
    }
};

#endif