#include "Hooks.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "LocalisationManager.hpp"

bool LocalisationMenuLayer::init()
{
    if (!MenuLayer::init())
        return false;

    auto path = Mod::get()->getResourcesDir() / "en-AU.json";

    if (std::filesystem::exists(path))
    {
        LocalisationManager::get()->loadLocalisationFile(path);
    }

    return true;
}