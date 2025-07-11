#include "../../Client/Module.hpp"
#include <Geode/modify/HardStreak.hpp>

using namespace geode::prelude;

class NoWavePulse : public Module
{
    public:
        MODULE_SETUP(NoWavePulse)
        {
            setName("No Wave Pulse");
            setID("no-wave-pulse");
            setCategory("Cosmetic");
            setDescription("No Wave Pulse help");
        }
};

SUBMIT_HACK(NoWavePulse);

class $modify (HardStreak)
{
    void updateStroke(float unused)
    {
        if (NoWavePulse::get()->getRealEnabled())
            m_pulseSize = 1;

        HardStreak::updateStroke(unused);
    }
};