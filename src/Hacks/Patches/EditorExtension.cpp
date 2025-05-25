#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../../Client/Client.h"
#include "../../Utils/OffsetManager.hpp"

using namespace geode::prelude;

$execute
{
    geode::ByteVector array = { 0xFF, 0xFF, 0xFF, 0xFF };
    std::vector<geode::Patch*> patches = {};

    if (auto offset = OffsetManager::get()->offsetForType(PatchType::EditorExtension1); offset != 0x80085)
    {
        patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + offset), array));
    }

    if (auto offset = OffsetManager::get()->offsetForType(PatchType::EditorExtension2); offset != 0x80085)
    {
        patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + offset), array));
    }

    Loader::get()->queueInMainThread([patches]{
        for (auto patch : patches)
        {
            Module::get("editor-extension")->addPatch(patch);
        }
    });
};