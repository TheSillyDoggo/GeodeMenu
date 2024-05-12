#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/CCArray.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

std::vector<Patch*> toolboxpatches = {};

void updateToolboxPatches(bool tpsEnabled)
{    
    if (toolboxpatches.size() > 0)
    {
        for (auto catgirl : toolboxpatches)
        {
            Mod::get()->disownPatch(catgirl); // goodbye cutie you will be very missed :3c
        }

        toolboxpatches.clear();
    }

    if (tpsEnabled)
    {
        log::info("patching toolbox");

        #ifdef GEODE_IS_WINDOWS
        toolboxpatches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + (0xa82ad)), {0x3d, 0xFF, 0xFF, 0xFF, 0xFF}).unwrap()); // custom object count
        toolboxpatches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + (0xa820f)), {0x3d, 0xFF, 0xFF, 0xFF, 0xFF}).unwrap()); // object count
        #endif

        #ifdef GEODE_IS_IOS
        //toolboxpatches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + (0xa82ad)), {0x3d, 0x99, 0x99, 0x99, 0x99}).unwrap()); // custom object count
        //toolboxpatches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + (0xa820f)), {0x3d, 0x99, 0x99, 0x99, 0x99}).unwrap()); // object count
        #endif

        #ifdef GEODE_IS_ANDROID32
        toolboxpatches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + (0x3a0d82 - 0x10000)), {0xe0, 0xf0, 0x0b, 0xe7}).unwrap()); // custom object count
        //toolboxpatches.push_back(Mod::get()->patch(reinterpret_cast<void*>(geode::base::get() + (0xa820f)), {0x3d, 0x99, 0x99, 0x99, 0x99}).unwrap()); // object count
        #endif
    }
}

class ToolboxChangedDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled)
    {
        updateToolboxPatches(Client::GetModuleEnabled("custom-obj-limit"));
    }
};

$execute
{
    Loader::get()->queueInMainThread([] {
        auto del = new ToolboxChangedDelegate();

        Client::GetModule("custom-obj-limit")->delegate = del;

        updateToolboxPatches(Client::GetModuleEnabled("custom-obj-limit"));
    });
}