#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoTrailBlending : public Module
{
    public:
        MODULE_SETUP(NoTrailBlending)
        {
            setName("No Trail Blending");
            setID("no-trail-blending");
            setCategory("Cosmetic");
            setDescription("No particle help");
        }
};

SUBMIT_HACK(NoTrailBlending);

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (m_regularTrail)
        {
            if (NoTrailBlending::get()->getRealEnabled())
                m_regularTrail->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
            else
                m_regularTrail->setBlendFunc({ 770, 1 });
        }
    }
};