#include "../../Client/InputModule.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class WaveTrailSize : public Module
{
    public:
        MODULE_SETUP(WaveTrailSize)
        {
            setName("Wave Trail Size");
            setID("wave-trail-size");
            setCategory("Cosmetic");
            setDescription("Change the size of the wave trail");
        }
};

class WaveTrailThickness : public InputModule
{
    public:
        MODULE_SETUP(WaveTrailThickness)
        {
            setName("Thickness");
            setID("wave-trail-size/thickness");
            setDescription("");

            setDefaultString("1.0");
            setPlaceholderString("Thickness");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

SUBMIT_HACK(WaveTrailSize);
SUBMIT_OPTION(WaveTrailSize, WaveTrailThickness);

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (m_waveTrail)
        {
            if (WaveTrailSize::get()->getRealEnabled())
                m_waveTrail->m_waveSize = WaveTrailThickness::get()->getStringFloat() * m_vehicleSize;
            else
                m_waveTrail->m_waveSize = m_vehicleSize;
        }
    }
};