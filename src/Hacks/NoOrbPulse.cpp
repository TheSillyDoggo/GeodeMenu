#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCCircleWave.hpp>
#include "../Client/Client.h"
#include "../Utils/Utils.hpp"

using namespace geode::prelude;

Module* orbPulse = nullptr;

class $modify (CCCircleWave)
{
    virtual void draw()
    {
        if (!orbPulse)
            orbPulse = Client::GetModule("no-circles");

        if (!PlayLayer::get() || !orbPulse->enabled) // if it isn't enabled or it isn't the level
            CCCircleWave::draw();
    }
};

#ifdef GEODE_IS_WINDOWS
#define offset 0x178
#elif GEODE_IS_ANDROID32
#define offset 0x16c
#elif GEODE_IS_ANDROID64
#define offset 0x1f8
#elif GEODE_IS_MACOS
#define offset 0x1c8
#elif GEODE_IS_IOS
#define offset 0x1c8
#endif

class $modify (PlayLayer)
{
    virtual void updateVisibility(float p0)
    {
        float v = MBO(float, FMODAudioEngine::sharedEngine(), offset);

        MBO(float, FMODAudioEngine::sharedEngine(), offset) = 1.0f;

        PlayLayer::updateVisibility(p0);

        MBO(float, FMODAudioEngine::sharedEngine(), offset) = v;
    }

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("PlayLayer::updateVisibility").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("no-orb-pulse");

            for (auto hook : hooks)
            {
                hook->setAutoEnable(false);

                if (!modu->enabled)
                    hook->disable();

                modu->hooks.push_back(hook);
            }
        });
    }
};

#undef offset