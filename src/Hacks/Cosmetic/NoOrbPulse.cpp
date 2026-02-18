#include "../../Client/Module.hpp"
#include "../../Client/FloatSliderModule.hpp"
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

class NoOrbPulseSize : public FloatSliderModule
{
    public:
        MODULE_SETUP(NoOrbPulseSize)
        {
            setID("no-orb-pulse/size");
            setDefaultValue(0.7f);
            setSnapValues({0.7f});
            setRange(0.2f, 1.0f);
        }
};

SUBMIT_HACK(NoOrbPulse);
SUBMIT_OPTION(NoOrbPulse, NoOrbPulseSize);

class $modify (PlayLayer)
{
    virtual void updateVisibility(float dt)
    {
        auto fmod = FMODAudioEngine::get();
        auto pulse1 = fmod->m_pulse1;
        auto pulse2 = m_audioEffectsLayer->m_audioScale;

        if (NoOrbPulse::get()->getRealEnabled())
        {
            fmod->m_pulse1 = NoOrbPulseSize::get()->getValue();
            m_audioEffectsLayer->m_audioScale = NoOrbPulseSize::get()->getValue();
        }

        PlayLayer::updateVisibility(dt);

        fmod->m_pulse1 = pulse1;
        m_audioEffectsLayer->m_audioScale = pulse2;
    }
};