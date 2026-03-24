#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"
#include <Utils.hpp>

using namespace geode::prelude;

class BackButton : public ButtonModule
{
    public:
        MODULE_SETUP(BackButton)
        {
            setID("shortcut/back-button");
            setCategory("Shortcuts");

            setPriority(sortPriority + 3);
        }

        virtual void onClick()
        {
            CCDirector::get()->getKeypadDispatcher()->dispatchKeypadMSG(ccKeypadMSGType::kTypeBackClicked);
        }

        virtual std::string getNotificationString()
        {
            return "";
        }
};

SUBMIT_HACK(BackButton);
