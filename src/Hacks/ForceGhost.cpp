#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    void toggleGhostEffect(GhostType p0)
    {
        if (PlayLayer::get())
        {
            if (Module::get("ghost-on")->enabled)
                p0 = GhostType::Enabled;

            if (Module::get("ghost-off")->enabled)
                p0 = GhostType::Disabled;
        }

        PlayerObject::toggleGhostEffect(p0);
    }
};