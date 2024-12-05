#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"
#include "../Utils/OffsetManager.hpp"

using namespace geode::prelude;

#ifndef QOLMOD_TPS_BYPASS_HOOK

std::vector<Patch*> patches;

void updateTPSPatches(bool tpsEnabled) {
    
    for (auto* patch : patches) {
        if (patch) Mod::get()->disownPatch(patch);
    }
    patches.clear();

    if (!tpsEnabled) return;

    constexpr float defaultTPS = 240.0f;
    float tps = defaultTPS;

    if (auto input = dynamic_cast<InputModule*>(Client::GetModule("tps-bypass")->options[0])) {
        if (auto parsedTPS = numFromString<float>(input->text); parsedTPS.isOk()) {
            tps = parsedTPS.unwrapOr(defaultTPS);
        }
    }

    if (auto offset = OffsetManager::get()->offsetForType(PatchType::PhysicsBypass); offset != 0x80085) {
        #ifdef GEODE_IS_WINDOWS
        auto array = geode::toBytes<float>(1.0f / tps);
        #else
        auto array = geode::toBytes<double>(1.0f / tps);
        #endif

        patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + offset), array));
    }
}

class TPSChangedDelegate : public ModuleChangeDelegate {
    void onModuleChanged(bool enabled) override {
        updateTPSPatches(Client::GetModuleEnabled("tps-bypass"));
    }
};

$execute {
    Loader::get()->queueInMainThread([] {
        auto* delegate = new TPSChangedDelegate();
        auto* tpsModule = Client::GetModule("tps-bypass");

        tpsModule->delegate = delegate;
        tpsModule->options[0]->delegate = delegate;

        updateTPSPatches(Client::GetModuleEnabled("tps-bypass"));
    });
}

#endif
