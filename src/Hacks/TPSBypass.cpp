#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"
#include "../Utils/OffsetManager.hpp"

using namespace geode::prelude;

#ifdef QOLMOD_TPS_BYPASS_HOOK

#else

std::vector<Patch*> patches = {};

void updateTPSPatches(bool tpsEnabled)
{    
    if (patches.size() > 0)
    {
        for (auto catgirl : patches)
        {
            if (catgirl)
                (void)Mod::get()->disownPatch(catgirl); // goodbye cutie you will be very missed :3c
        }

        patches.clear();
    }

    float tps = 240;

    auto x = numFromString<float>(as<InputModule*>(Client::GetModule("tps-bypass")->options[0])->text);

    if (x.isOk())
    {
        tps = x.unwrapOr(240);
    }

    if (tpsEnabled)
    {
        if (auto offset = OffsetManager::get()->offsetForType(PatchType::PhysicsBypass); offset != 0x80085)
        {
            #ifdef GEODE_IS_WINDOWS
            auto array = geode::toBytes<float>(1.0f / tps);
            #else
            auto array = geode::toBytes<double>(1.0f / tps);
            #endif

            patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + offset), array));
        }
    }
}

class TPSChangedDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled)
    {
        updateTPSPatches(Client::GetModuleEnabled("tps-bypass"));
    }
};

$execute
{
    Loader::get()->queueInMainThread([] {
        auto del = new TPSChangedDelegate();

        Client::GetModule("tps-bypass")->delegate = del;
        Client::GetModule("tps-bypass")->options[0]->delegate = del;

        updateTPSPatches(Client::GetModuleEnabled("tps-bypass"));
    });
}

#endif