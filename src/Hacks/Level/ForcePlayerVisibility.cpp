#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class ForceShowPlayer : public Module
{
    public:
        MODULE_SETUP(ForceShowPlayer)
        {
            setName("Force Show Player");
            setID("show-player");
            setCategory("Level");
            setDescription("Forces all players to be visible");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

class ForceHidePlayer : public Module
{
    public:
        MODULE_SETUP(ForceHidePlayer)
        {
            setName("Force Hide Player");
            setID("hide-player");
            setCategory("Level");
            setDescription("Forces all players to be invisible");
        }
};

SUBMIT_HACK(ForceShowPlayer);
SUBMIT_HACK(ForceHidePlayer);

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (ForceShowPlayer::get()->getRealEnabled())
            setVisible(true);

        if (ForceHidePlayer::get()->getRealEnabled())
            setVisible(false);
    }
};