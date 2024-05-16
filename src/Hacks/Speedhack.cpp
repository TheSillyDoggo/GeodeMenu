#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;
FMOD::DSP *pitchDSP = nullptr;
InputModule* pitchAmount;
float lastPitch = 0;

float speedhackLogic(float dt)
{
    #ifndef GEODE_IS_IOS
    if (!masterGroup)
    {
        FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);
    }
    #endif

    if (!pitchAmount)
        pitchAmount = as<InputModule*>(Client::GetModule("pitch-shifter")->options[0]);

    if (!masterGroup || !pitchAmount)
        return dt;

    if (lastPitch != (Client::GetModuleEnabled("pitch-shifter") ? pitchAmount->getFloatValue() : 1))
    {
        if (pitchDSP)
            masterGroup->removeDSP(pitchDSP);

        FMODAudioEngine::sharedEngine()->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchDSP);
        masterGroup->addDSP(0, pitchDSP);
        lastPitch = Client::GetModuleEnabled("pitch-shifter") ? pitchAmount->getFloatValue() : 1;

        pitchDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, Client::GetModuleEnabled("pitch-shifter") ? pitchAmount->getFloatValue() : 1);
    }

    ColourUtility::totalSessionTime += dt;

    if (SpeedhackTop::instance)
    {
        if (SpeedhackEnabled::instance->enabled)
        {
            bool m = SpeedhackMus::instance->enabled;

            float v = SpeedhackTop::instance->getFloatValue();

            if (SpeedhackGameplay::instance->enabled)
                if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

            #ifndef GEODE_IS_IOS //todo: this is dumb
            masterGroup->setPitch(SpeedhackMus::instance->enabled ? v : 1);
            #endif
            ColourUtility::update(dt * v);
            return dt * v;
        }
    }

    masterGroup->setPitch(1);
    ColourUtility::update(dt);
    return dt;
}

#ifdef GEODE_IS_MACOS

class $modify (CCScheduler)
{
    virtual void update(float dt)
    {
        dt = speedhackLogic(dt);

        CCScheduler::update(dt);
    }
};

#else

#ifdef GEODE_IS_IOS

//

FMOD_RESULT MenuLayer_onNewgrounds(FMOD::System* self, const char *name, FMOD::ChannelGroup **channelgroup) {
    //auto res = self->createChannelGroup(name, channelgroup);
    auto res = reinterpret_cast<FMOD_RESULT(__thiscall*)(FMOD::System*, const char*, FMOD::ChannelGroup**)>(geode::base::get() + 0x4c8964)(self, name, channelgroup);

    if (!masterGroup)
        masterGroup = *channelgroup;

    log::info("WE HAVE A MASTER GROUP LETS FUCKING GO!!!!");

    return res;
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::get() + 0x4c8964), // address
        &MenuLayer_onNewgrounds, // detour
        "FMOD::System::createChannelGroup", // display name, shows up on the console
        tulip::hook::TulipConvention::Thiscall // calling convention
    );
}

#endif

void myUpdate(CCScheduler* ins, float dt)
{
    dt = speedhackLogic(dt);
    
    ins->update(dt);
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getVirtual(&CCScheduler::update)
        ),
        &myUpdate,
        "cocos2d::CCScheduler::update",
        tulip::hook::TulipConvention::Thiscall
    );
}

#endif