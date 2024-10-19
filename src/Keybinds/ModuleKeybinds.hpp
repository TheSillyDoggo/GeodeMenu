#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

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