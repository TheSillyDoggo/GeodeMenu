#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "Speedhack.hpp"

using namespace geode::prelude;

FMOD::DSP *pitchDSP = nullptr;
InputModule* pitchAmount;

void updatePitch()
{
    if (pitchDSP)
        masterGroup->removeDSP(pitchDSP);

    if (!Module::get("pitch-shifter")->enabled)
        return;

    FMODAudioEngine::sharedEngine()->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchDSP);
    masterGroup->addDSP(0, pitchDSP);

    pitchDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, Module::get("pitch-shifter")->enabled ? pitchAmount->getFloatValue() : 1);
}

class PitchChangedDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled)
    {
        updatePitch();
    }
};

$execute
{
    Loader::get()->queueInMainThread([] {
        auto del = new PitchChangedDelegate();

        Module::get("pitch-shifter")->delegate = del;
        Module::get("pitch-shifter")->options[0]->delegate = del;
        pitchAmount = as<InputModule*>(Module::get("pitch-shifter")->options[0]);

        Loader::get()->queueInMainThread([] {
            updatePitch();
        });
    });
}