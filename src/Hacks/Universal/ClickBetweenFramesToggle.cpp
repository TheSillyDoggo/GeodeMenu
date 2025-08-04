#include "../../Client/ModBoolModule.hpp"
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class CBFToggle : public ModBoolModule
{
    public:
        MODULE_SETUP(CBFToggle)
        {
            setName("Click between frames");
            setCategory("Universal");
            setDescription("Toggles the \"CBF Enabled\" option in the cbf mod settings");
            setOptionPointer("syzzi.click_between_frames", "soft-toggle");
            setInvert(true);

            if (!mod || !mod->shouldLoad())
            {
                setDisabled(true);
                setDisabledMessage(mod ? "This mod requires <cc>click between frames</c> to be enabled" : "This mod requires <cc>click between frames</c> to be installed");
            }
        }
};

SUBMIT_HACK(CBFToggle);