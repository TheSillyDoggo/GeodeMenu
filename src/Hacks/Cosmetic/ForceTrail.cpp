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

class TrailOffWaveOnly : public Module
{
    public:
        MODULE_SETUP(TrailOffWaveOnly)
        {
            setName("Wave Only");
            setID("trail-off/wave-only");
            setDescription("Force the trail off only when in the wave gamemode");
        }
};

SUBMIT_HACK(ForceTrailOn);
SUBMIT_HACK(ForceTrailOff);
SUBMIT_OPTION(ForceTrailOff, TrailOffWaveOnly);

class $modify (PlayerObject)
{
    virtual void update(float delta)
    {
        PlayerObject::update(delta);

        if (m_regularTrail)
        {
            bool en = m_regularTrail->m_bStroke;

            if (ForceTrailOn::get()->getRealEnabled())
                en = true;

            if (ForceTrailOff::get()->getRealEnabled())
            {
                if (TrailOffWaveOnly::get()->getRealEnabled())
                {
                    en = m_isDart ? false : en;
                }
                else
                {
                    en = false;
                }
            }
            
            m_regularTrail->m_bStroke = en;
        }
    }
};