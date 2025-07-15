#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoDashFire : public Module
{
    public:
        MODULE_SETUP(NoDashFire)
        {
            setName("No Dash Fire");
            setID("no-dash-fire");
            setCategory("Cosmetic");
            setDescription("No dash fire");
        }
};

SUBMIT_HACK(NoDashFire);

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (NoDashFire::get()->getRealEnabled())
        {
            if (m_dashFireSprite)
                m_dashFireSprite->setVisible(false);
        }
    }
};