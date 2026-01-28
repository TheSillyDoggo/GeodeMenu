#pragma once

#include <Module.hpp>
#include <InputModule.hpp>
#include <ButtonModule.hpp>
#include <BlurAPI.hpp>

class NotificationsEnabled : public Module
{
    public:
        MODULE_SETUP(NotificationsEnabled)
        {
            setID("notifications");
            setCategory("Config/Menu");
            setDefaultEnabled(true);
        }
};

class NotificationsTestButton : public ButtonModule
{
    public:
        MODULE_SETUP(NotificationsTestButton)
        {
            setID("notifications/test");
            setPriority(1);
        }

        virtual void onClick();
};

class NotificationsDuration : public InputModule
{
    public:
        MODULE_SETUP(NotificationsDuration)
        {
            setID("notifications/duration");
            setPriority(2);

            setDefaultString("2.5");
            setPlaceholderString("2.5");
            setHint("seconds");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

class NotificationsRight : public Module
{
    public:
        MODULE_SETUP(NotificationsRight)
        {
            setID("notifications/right");
            setPriority(3);
        }
};

class NotificationsBlur : public Module
{
    public:
        MODULE_SETUP(NotificationsBlur)
        {
            setID("notifications/blur");
            setPriority(4);

            if (!BlurAPI::willLoad())
            {
                setDisabled(true);
                setDisabledMessage("This requires the mod <cc>Blur API</c> to be installed");
            }
        }
};