#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

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
            float v = 1.0f;

            auto x = numFromString<float>(SpeedhackTop::instance->text);

            if (x.isOk())
            {
                v = x.value();
            }

            if (v < 0.01f)
                v = 0.01f;

            if (v > 99999)
                v = 99999;

            bool m = SpeedhackMus::instance->enabled;

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
    auto res = self->createChannelGroup(name, channelgroup);

    if (!masterGroup)
        masterGroup = *channelgroup;

    return res;
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::get() + 0x4c8964), // address
        &MenuLayer_onNewgrounds, // detour
        "MenuLayer::onNewgrounds", // display name, shows up on the console
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