#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"
#include <BetterAlertLayer.hpp>
#include <LocalisationManager.hpp>
#include <Utils.hpp>

using namespace geode::prelude;

class CrashTheGame : public ButtonModule, public FLAlertLayerProtocol
{
    public:
        MODULE_SETUP(CrashTheGame)
        {
            setID("shortcut/crash-the-game");
            setCategory("Shortcuts");
        }

        virtual void onClick()
        {
            auto alert = BetterAlertLayer::createWithLocalisation(this, "names/shortcut/crash-the-game", "ui/crash-confirmation", "ui/cancel-button", "ui/confirm-button", 300, false, 67, 1.0f);
            alert->show();
        }

        virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2)
        {
            if (btn2)
            {
                log::error("CRASHING THE GAME!");

                reinterpret_cast<CCNode*>(0x676980085)->CCNode::visit();
            }
        }
};

SUBMIT_HACK(CrashTheGame);
