#pragma once

#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"

class RestartAttempt : public ButtonModule
{
    public:
        MODULE_SETUP(RestartAttempt)
        {
            setID("shortcut/restart-attempt");
            setCategory("Level");

            setPriority(sortPriority + 1);
        }

        virtual void onClick()
        {
            if (PlayLayer::get())
                PlayLayer::get()->resetLevel();
        }
};

SUBMIT_HACK(RestartAttempt);