#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class ForceGhostOn : public Module
{
    public:
        MODULE_SETUP(ForceGhostOn)
        {
            setName("Force Ghost On");
            setID("ghost-on");
            setCategory("Cosmetic");
            setDescription("Forces the ghost trail on");
        }
};

class ForceGhostOff : public Module
{
    public:
        MODULE_SETUP(ForceGhostOff)
        {
            setName("Force Ghost Off");
            setID("ghost-off");
            setCategory("Cosmetic");
            setDescription("Forces the ghost trail off");
        }
};

SUBMIT_HACK(ForceGhostOn);
SUBMIT_HACK(ForceGhostOff);

class $modify (PlayerObject)
{
    void toggleGhostEffect(GhostType state)
    {
        if (PlayLayer::get())
        {
            if (ForceGhostOn::get()->getRealEnabled())
                state = GhostType::Enabled;

            if (ForceGhostOff::get()->getRealEnabled())
                state = GhostType::Disabled;
        }

        PlayerObject::toggleGhostEffect(state);
    }
};