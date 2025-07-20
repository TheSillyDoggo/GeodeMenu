#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoRobotFire : public Module
{
    public:
        MODULE_SETUP(NoRobotFire)
        {
            setName("No Robot Fire");
            setID("no-robot-fire");
            setCategory("Cosmetic");
            setDescription("Hides the fire under the robot while it's boosting");
        }
};

SUBMIT_HACK(NoRobotFire);

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (NoRobotFire::get()->getRealEnabled())
        {
            m_robotFire->setVisible(false);
            m_robotBurstParticles->setVisible(false);
        }
    }
};