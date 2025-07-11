#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoPlayerRotation : public Module
{
    public:
        MODULE_SETUP(NoPlayerRotation)
        {
            setName("No Rotation");
            setID("no-rot");
            setCategory("Cosmetic");
            setDescription("Disables playwr rotation");
        }
};

SUBMIT_HACK(NoPlayerRotation);

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (NoPlayerRotation::get()->getRealEnabled())
            setRotation(0);
    }
};