#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"

class RestartAttempt : public ButtonModule
{
    public:
        MODULE_SETUP(RestartAttempt)
        {
            setID("shortcut/restart-attempt");
            setCategory("Shortcuts");

            setPriority(sortPriority + 1);
        }

        virtual void onClick()
        {
            if (PlayLayer::get())
                PlayLayer::get()->PlayLayer::resetLevel();
        }
};

SUBMIT_HACK(RestartAttempt);