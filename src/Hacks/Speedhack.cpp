#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "Speedhack.hpp"

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;

float speedhackLogic(float dt)
{
    #ifndef GEODE_IS_IOS
    if (!masterGroup)
        FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);
    #endif

    if (!masterGroup)
        return dt;

    ColourUtility::totalSessionTime += dt;

    if (SpeedhackTop::instance)
    {
        if (SpeedhackEnabled::instance->enabled)
        {
            bool m = SpeedhackMus::instance->enabled;

            float v = SpeedhackTop::instance->getFloatValue();

            if (SpeedhackGameplay::instance->enabled)
                if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

            #ifdef GEODE_IS_IOS
            reinterpret_cast<FMOD_RESULT(__thiscall*)(FMOD::ChannelControl*, float)>(geode::base::get() + 0x4ffec4)(masterGroup, SpeedhackMus::instance->enabled ? v : 1);
            #else
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

class $modify (CCScheduler)
{
    virtual void update(float dt)
    {
        dt = speedhackLogic(dt);

        CCScheduler::update(dt);
    }
};

#ifdef GEODE_IS_IOS

FMOD_RESULT FMOD_System_createChannelGroup(FMOD::System* self, const char *name, FMOD::ChannelGroup **channelgroup) {
    auto res = reinterpret_cast<FMOD_RESULT(__thiscall*)(FMOD::System*, const char*, FMOD::ChannelGroup**)>(geode::base::get() + 0x4c8964)(self, name, channelgroup);

    if (!masterGroup)
        masterGroup = *channelgroup;

    log::info("WE HAVE A MASTER GROUP LETS FUCKING GO!!!!");

    return res;
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::get() + 0x4c8964), // address
        &FMOD_System_createChannelGroup, // detour
        "FMOD::System::createChannelGroup", // display name, shows up on the console
        tulip::hook::TulipConvention::Thiscall // calling convention
    );
}

#endif