/*#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

$execute
{
    std::vector<geode::Patch*> patches = {};

    #ifdef GEODE_IS_WINDOWS

    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x5ed33c), { 0x1, 0x0, 0x0, 0x0 }));

    #endif

    Loader::get()->queueInMainThread([patches]{
        for (auto patch : patches)
        {
            Client::GetModule("editor-extension")->addPatch(patch);
        }
    });
};*/
//win:
//0x5ed33c
//0x