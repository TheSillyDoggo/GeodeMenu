#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "Speedhack.hpp"

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;

float speedhackLogic(float dt)
{
    if (CCScene::get() && CCScene::get()->getChildByType<LoadingLayer>(0))
        return dt;

    if (!masterGroup)
        FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);

    if (!masterGroup)
        return dt;

    ColourUtility::totalSessionTime += dt;

    if (SpeedhackTop::instance)
    {
        if (SpeedhackEnabled::instance->enabled)
        {
            bool m = SpeedhackMus::instance->enabled;

            float v = SpeedhackTop::instance->getFloatValue();

            masterGroup->setPitch((SpeedhackGameplay::instance->enabled ? GJBaseGameLayer::get() && SpeedhackMus::instance->enabled : SpeedhackMus::instance->enabled) ? v : 1);

            if (SpeedhackGameplay::instance->enabled)
            {
                ColourUtility::update(dt);
                return dt;
            }

            ColourUtility::update(dt * v);

            // okay, time to explain myself.
            // CBF (cheat between frames) has a setting called Physics Bypass. This breaks the speedhack in qolmod.
            // because of this i have to do this :c
            #ifdef GEODE_IS_WINDOWS
            static bool cbfChecked = false;
            static bool cbfEnabled = false;
            static Mod* cbf = nullptr;

            if (!cbfChecked)
            {
                cbf = Loader::get()->getInstalledMod("syzzi.click_between_frames");

                if (cbf)
                    cbfEnabled = cbf->shouldLoad() || cbf->isEnabled();

                cbfChecked = true;
            }

            if (cbfEnabled && cbf->getSettingValue<bool>("actual-delta"))
            {
                CCDirector::get()->setActualDeltaTime(CCDirector::get()->getActualDeltaTime() * v);
                CCDirector::get()->setDeltaTime(CCDirector::get()->getDeltaTime() * v);
            }
            #endif
            return dt * v;
        }
    }

    masterGroup->setPitch(1);
    ColourUtility::update(dt);
    return dt;
}

#if (defined(GEODE_IS_IOS) || defined(GEODE_IS_MACOS))

class $modify (CCScheduler)
{
    virtual void update(float dt)
    {
        dt = speedhackLogic(dt);

        CCScheduler::update(dt);
    }
};

#else

void myUpdate(CCScheduler* ins, float dt)
{
    dt = speedhackLogic(dt);
    
    ins->update(dt);
}

$execute {
    (void)Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getVirtual(&CCScheduler::update)
        ),
        &myUpdate,
        "cocos2d::CCScheduler::update",
        tulip::hook::TulipConvention::Thiscall
    );
}

#endif

#ifdef GEODE_IS_IOS

FMOD_RESULT FMOD_System_createChannelGroup(FMOD::System* self, const char *name, FMOD::ChannelGroup **channelgroup) {
    auto res = reinterpret_cast<FMOD_RESULT(__cdecl*)(FMOD::System*, const char*, FMOD::ChannelGroup**)>(geode::base::get() + 0x4d4f1c)(self, name, channelgroup);

    if (!masterGroup)
        masterGroup = *channelgroup;

    log::info("WE HAVE A MASTER GROUP LETS FUCKING GO!!!!");

    return res;
}

$execute {
    (void)Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::get() + OffsetManager::get()->offsetForFunction(FunctionType::FMOD__System__createChannelGroup)), // address
        &FMOD_System_createChannelGroup, // detour
        "FMOD::System::createChannelGroup", // display name, shows up on the console
        tulip::hook::TulipConvention::Cdecl // calling convention
    );
}

#endif

class $modify (GJBaseGameLayer)
{
    virtual void update(float dt)
    {
        if (SpeedhackEnabled::instance->enabled && SpeedhackGameplay::instance->enabled)
            dt *= SpeedhackTop::instance->getFloatValue();

        GJBaseGameLayer::update(dt);
    }
};