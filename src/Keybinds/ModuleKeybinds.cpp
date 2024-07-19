#include "ModuleKeybinds.hpp"

bool KeybindsMenuLayer::init()
{
    if (QOLModKeybinds::isKeybindsLoaded() && !QOLModKeybinds::hasSetupKeybinds)
        QOLModKeybinds::setupKeybinds();

    log::info("loaded keybinds: {}", QOLModKeybinds::isKeybindsLoaded());

    return MenuLayer::init();
}

bool QOLModKeybinds::isKeybindsLoaded()
{
    return Loader::get()->getLoadedMod("geode.custom-keybinds");
}

void QOLModKeybinds::setupKeybinds()
{
    hasSetupKeybinds = true;

    log::info("setting up keybinds");

    /*BindManager::get()->registerBindable({
        // ID, should be prefixed with mod ID
        "backlip"_spr,
        // Name
        "Do a Backflip!",
        // Description, leave empty for none
        "Throw a backflip",
        // Default binds
        { Keybind::create(KEY_Q, Modifier::None) },
        // Category; use slashes for specifying subcategories. See the
        // Category class for default categories
        "My Mod/Awesome Tricks"
    });

    new EventListener([=](InvokeBindEvent* event) {
        


	    return ListenerResult::Propagate;
    }, InvokeBindFilter(nullptr, "backlip"_spr));*/

    log::info("setup complete");
}