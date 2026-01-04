#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class NoOrbPulse : public Module
{
    public:
        MODULE_SETUP(NoOrbPulse)
        {
            setName("No Orb Pulse");
            setID("no-orb-pulse");
            setCategory("Cosmetic");
            setDescription("Disables orb's from pulsing, Doesn't work on main levels made before 2.2");
        }
};

SUBMIT_HACK(NoOrbPulse);

class $modify (PlayLayer)
{
    virtual void updateVisibility(float dt)
    {
        auto fmod = FMODAudioEngine::get();
        auto pulse1 = fmod->m_pulse1;
        auto pulse2 = m_audioEffectsLayer->m_audioScale;

        if (NoOrbPulse::get()->getRealEnabled())
        {
            fmod->m_pulse1 = 1.0f;
            m_audioEffectsLayer->m_audioScale = 1.0f;
        }

        PlayLayer::updateVisibility(dt);

        fmod->m_pulse1 = pulse1;
        m_audioEffectsLayer->m_audioScale = pulse2;
    }
};