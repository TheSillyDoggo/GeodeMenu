#include "Hooks.hpp"
#include "NotificationManager.hpp"
#include "Modules/Modules.hpp"

using namespace geode::prelude;

void NotificationEGLView::swapBuffers()
{
    if (NotificationsEnabled::get()->getRealEnabled())
        NotificationManager::get()->visit();

    CCEGLView::swapBuffers();
}