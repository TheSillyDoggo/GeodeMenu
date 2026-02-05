#include "Modules.hpp"
#include "../NotificationManager.hpp"
#include "../../Localisation/LocalisationManager.hpp"

/./SUBMIT_HACK(NotificationsEnabled);
SUBMIT_OPTION(NotificationsEnabled, NotificationsTestButton);
SUBMIT_OPTION(NotificationsEnabled, NotificationsDuration);
SUBMIT_OPTION(NotificationsEnabled, NotificationsRight);
SUBMIT_OPTION(NotificationsEnabled, NotificationsBlur);

void NotificationsTestButton::onClick()
{
    NotificationManager::get()->notifyToast(LocalisationManager::get()->getLocalisedString("ui/notification-test"));
}