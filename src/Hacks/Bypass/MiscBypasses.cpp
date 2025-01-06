/*#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

namespace MiscBypasses
{
    std::vector<Patch*> zoompatches = {};

    Patch* safeGetPatch(int address, const geode::ByteVector &data)
    {
        auto patch = Mod::get()->patch(reinterpret_cast<void*>(address), data);

        if (patch.isOk() && patch.unwrap())
            return patch.unwrap();

        log::error("Error patching {}, {}", address - base::get(), patch.error());
        return nullptr;
    }
};

using namespace MiscBypasses;

void updateZoom()
{
    if (zoompatches.size() == 0)
    {
    #ifdef GEODE_IS_WINDOWS
        
        zoompatches.push_back(safeGetPatch(geode::base::get() + 0x1b936a, {0x90, 0x90}));
        //zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x1b939a), {0x90, 0x90, 0x90}).unwrap());
        //zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x1B9390), {0x90, 0x90, 0x90}).unwrap());

    #elif GEODE_IS_MACOS

    #elif GEODE_IS_IOS

        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x201130), {0x90, 0x90, 0x90, 0x90}).unwrap());
        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x201144), {0x90, 0x90, 0x90, 0x90}).unwrap());
        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x201138), {0x90, 0x90, 0x90, 0x90}).unwrap());

    #elif GEODE_IS_ANDROID32
        
        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x478bdc - 0x10000), {0x90, 0x90, 0x90, 0x90}).unwrap());
        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x478bce - 0x10000), {0x90, 0x90, 0x90, 0x90}).unwrap());
        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x478bda - 0x10000), {0x90, 0x90}).unwrap());

    #elif GEODE_IS_ANDROID64

        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x94cf54 - 0x100000), {0x90, 0x90, 0x90, 0x90}).unwrap());
        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x94cfbc - 0x100000), {0x90, 0x90, 0x90, 0x90}).unwrap());
        zoompatches.push_back(Mod::get()->patch(reinterpret_cast<void *>(geode::base::get() + 0x94cf6c - 0x100000), {0x90, 0x90, 0x90, 0x90}).unwrap());

    #endif

        for (auto patch : zoompatches)
        {
            patch->setAutoEnable(false);
        }
    }

    for (auto patch : zoompatches)
    {
        if (Client::GetModuleEnabled("zoom-limit"))
            patch->enable();
        else
            patch->disable();
    }
}

class MiscChangedDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled)
    {
        if (!enabled)
            return;

        updateZoom();
    }
};

$execute
{
    Loader::get()->queueInMainThread([] {
        auto del = new MiscChangedDelegate();
        auto misc = Client::GetModule("misc-bypass");

        misc->delegate = del;
        misc->options[0]->delegate = del;

        if (!misc->enabled)
            return;

        updateZoom();
    });
}*/