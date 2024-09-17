#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

$execute
{
    // first address is found in CCPoint * EditorUI::getLimitedPosition(EditorUI *this,CCPoi nt *__return,CCPoint param_3)
    // second address is found in void EditorUI::constrainGameLayerPosition(EditorUI *this,fl oat param_2,float param_3)

    std::vector<geode::Patch*> patches = {};

    #ifdef GEODE_IS_WINDOWS
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x5ed33c), { 0xFF, 0xFF, 0xFF, 0xFF }));
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x5ed340), { 0xFF, 0xFF, 0xFF, 0xFF }));
    #endif

    #ifdef GEODE_IS_ANDROID32
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + (0x37e678 - 0x10000)), { 0xFF, 0xFF, 0xFF, 0xFF }));
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + (0x37ea1c - 0x10000)), { 0xFF, 0xFF, 0xFF, 0xFF }));
    #endif

    #ifdef GEODE_IS_ANDROID64
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + (0x76cdb4 - 0x100000)), { 0xFF, 0xFF, 0xFF, 0xFF }));
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + (0x76d2fc - 0x100000)), { 0xFF, 0xFF, 0xFF, 0xFF }));
    #endif

    #ifdef GEODE_IS_INTEL_MAC
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x822770), { 0xFF, 0xFF, 0xFF, 0xFF }));
    patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + 0x82258c), { 0xFF, 0xFF, 0xFF, 0xFF }));
    #endif

    #ifdef GEODE_IS_ARM_MAC
    //patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + ), { 0xFF, 0xFF, 0xFF, 0xFF }));
    //patches.push_back(createPatchSafe(reinterpret_cast<void*>(geode::base::get() + ), { 0xFF, 0xFF, 0xFF, 0xFF }));
    #endif

    Loader::get()->queueInMainThread([patches]{
        for (auto patch : patches)
        {
            Client::GetModule("editor-extension")->addPatch(patch);
        }
    });
};
//win:
//0x5ed33c
//0x