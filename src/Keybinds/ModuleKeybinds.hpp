#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Keybinds.hpp>

using namespace geode::prelude;
// using namespace keybinds;

class $modify (KeybindsMenuLayer, MenuLayer)
{
    bool init();
};

namespace QOLModKeybinds
{
    bool hasSetupKeybinds;

    bool isKeybindsLoaded();

    void setupKeybinds();
};