#include "../../Client/InputModule.hpp"
#include <Geode/modify/CCTransitionFade.hpp>
#include "../Speedhack/Speedhack.hpp"

using namespace geode::prelude;

class PitchShifter : public Module
{
    protected:
        FMOD::DSP* pitchDSP = nullptr;

    public:
        MODULE_SETUP(PitchShifter)
        {
            setName("Pitch Shifter");
            setID("pitch-shifter");
            setCategory("Universal");
            setDescription("Shift the pitch of the games audio");
        }

        virtual void onToggle();
};

class PitchShifterOctaves : public InputModule
{
    public:
        MODULE_SETUP(PitchShifterOctaves)
        {
            setName("Pitch");
            setID("pitch-shifter/octaves");
            setDescription("");

            setDefaultString("12");
            setPlaceholderString("Pitch");
            setHint("semitones");

            setStringFilter("1234567890.");
            setMaxCharCount(4);
        }

        virtual void onToggle()
        {
            PitchShifter::get()->onToggle();
        }
};

void PitchShifter::onToggle()
{
    auto masterGroup = Speedhack::get()->getMasterChannel();

    if (pitchDSP)
    {
        masterGroup->removeDSP(pitchDSP);
        pitchDSP = nullptr;
    }

    if (!getRealEnabled())
        return;

    FMODAudioEngine::sharedEngine()->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchDSP);
    masterGroup->addDSP(0, pitchDSP);

    pitchDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, PitchShifterOctaves::get()->getStringFloat() / 12.0f);
}

SUBMIT_HACK(PitchShifter);
SUBMIT_OPTION(PitchShifter, PitchShifterOctaves);