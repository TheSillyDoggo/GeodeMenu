#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class ForceTrailOn : public Module
{
    public:
        MODULE_SETUP(ForceTrailOn)
        {
            setName("Force Trail On");
            setID("trail-on");
            setCategory("Cosmetic");
            setDescription("Force the trail on");
        }
};

class ForceTrailOff : public Module
{
    public:
        MODULE_SETUP(ForceTrailOff)
        {
            setName("Force Trail Off");
            setID("trail-off");
            setCategory("Cosmetic");
            setDescription("Force the trail off");
        }
};

SUBMIT_HACK(ForceTrailOn);
SUBMIT_HACK(ForceTrailOff);

class $modify (PlayerObject)
{
    virtual void update(float delta)
    {
        PlayerObject::update(delta);

        // 怖い

        if (m_regularTrail)
            m_regularTrail->m_bStroke = ForceTrailOn::get()->getRealEnabled() ? true : (ForceTrailOff::get()->getRealEnabled() ? false : m_regularTrail->m_bStroke);

        if (m_shipStreak)
            m_shipStreak->m_bStroke = ForceTrailOn::get()->getRealEnabled() ? true : (ForceTrailOff::get()->getRealEnabled() ? false : m_shipStreak->m_bStroke);
    }
};