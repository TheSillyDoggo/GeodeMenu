#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "Speedhack.hpp"

using namespace geode::prelude;

FMOD::DSP *pitchDSP = nullptr;
InputModule* pitchAmount;

void updatePitch()
{
    if (pitchDSP)
#ifdef GEODE_IS_IOS
        reinterpret_cast<FMOD_RESULT(__cdecl*)(FMOD::ChannelControl*, FMOD::DSP*)>(geode::base::get() + OffsetManager::get()->offsetForFunction(FunctionType::FMOD__ChannelControl__removeDSP))(masterGroup, pitchDSP); //removeDSP
#else
        masterGroup->removeDSP(pitchDSP);
#endif

    if (!Client::GetModuleEnabled("pitch-shifter"))
        return;

#ifdef GEODE_IS_IOS
    reinterpret_cast<FMOD_RESULT(__cdecl*)(FMOD::System*, FMOD_DSP_TYPE, FMOD::DSP**)>(geode::base::get() + OffsetManager::get()->offsetForFunction(FunctionType::FMOD__System__createDSPByType))(FMODAudioEngine::sharedEngine()->m_system, FMOD_DSP_TYPE_PITCHSHIFT, &pitchDSP); //createDSPByType
    reinterpret_cast<FMOD_RESULT(__cdecl*)(FMOD::ChannelControl*, int, FMOD::DSP*)>(geode::base::get() + OffsetManager::get()->offsetForFunction(FunctionType::FMOD::ChannelControl::addDSP))(masterGroup, 0, pitchDSP); //addDSP
#else
    FMODAudioEngine::sharedEngine()->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchDSP);
    masterGroup->addDSP(0, pitchDSP);
#endif

#ifdef GEODE_IS_IOS
    reinterpret_cast<FMOD_RESULT(__cdecl*)(FMOD::DSP*, int, float)>(geode::base::get() + OffsetManager::get()->offsetForFunction(FunctionType::FMOD__DSP__setParameterFloat))(pitchDSP, FMOD_DSP_PITCHSHIFT_PITCH, Client::GetModuleEnabled("pitch-shifter") ? pitchAmount->getFloatValue() : 1); //setParameterFloat
#else
    pitchDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, Client::GetModuleEnabled("pitch-shifter") ? pitchAmount->getFloatValue() : 1);
#endif
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

        Client::GetModule("pitch-shifter")->delegate = del;
        Client::GetModule("pitch-shifter")->options[0]->delegate = del;
        pitchAmount = as<InputModule*>(Client::GetModule("pitch-shifter")->options[0]);

        Loader::get()->queueInMainThread([] {
            updatePitch();
        });
    });
}