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
            setID("pitch-shifter");
            setCategory("Universal");
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

            setPriority(1);

            setStringFilter("1234567890.");
            setMaxCharCount(4);
        }

        virtual void onToggle()
        {
            PitchShifter::get()->onToggle();
        }
};

class PitchShifterMusicOnly : public Module
{
    public:
        MODULE_SETUP(PitchShifterMusicOnly)
        {
            setID("pitch-shifter/music-only");
            setPriority(2);
            setDefaultEnabled(true);
        }

        virtual void onToggle()
        {
            PitchShifter::get()->onToggle();
        }
};

void PitchShifter::onToggle()
{
    auto masterGroup = Speedhack::get()->getMasterChannel();
    auto musicChannel = FMODAudioEngine::get()->m_backgroundMusicChannel;

    if (pitchDSP)
    {
        masterGroup->removeDSP(pitchDSP);
        musicChannel->removeDSP(pitchDSP);
        pitchDSP = nullptr;
    }

    if (!getRealEnabled())
        return;

    FMODAudioEngine::sharedEngine()->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchDSP);

    if (PitchShifterMusicOnly::get()->getRealEnabled())
        musicChannel->addDSP(0, pitchDSP);
    else
        masterGroup->addDSP(0, pitchDSP);

    pitchDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, PitchShifterOctaves::get()->getStringFloat() / 12.0f);
}

SUBMIT_HACK(PitchShifter);
SUBMIT_OPTION(PitchShifter, PitchShifterOctaves);
SUBMIT_OPTION(PitchShifter, PitchShifterMusicOnly);

$on_mod (Loaded)
{
    PitchShifter::get()->onToggle();
}