#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoShipFire : public Module
{
    public:
        MODULE_SETUP(NoShipFire)
        {
            setName("No Ship Fire");
            setID("no-ship-fire");
            setCategory("Cosmetic");
            setDescription("Disables shaders");
        }
};

SUBMIT_HACK(NoShipFire);

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (NoShipFire::get()->getRealEnabled())
        {
            if (m_shipStreak)
                m_shipStreak->setVisible(false);
        }
    }
};