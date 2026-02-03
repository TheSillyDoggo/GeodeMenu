#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <NotificationManager.hpp>
#include <Modules/Modules.hpp>
#include "../../GUI/FloatingButton/FloatingUIManager.hpp"
#include "../../GUI/AndroidUI.hpp"
#include "../../GUI/AndroidBall.hpp"

using namespace geode::prelude;

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