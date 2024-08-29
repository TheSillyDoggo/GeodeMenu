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
            if (Client::GetModuleEnabled("ghost-on"))
                p0 = GhostType::Enabled;

            if (Client::GetModuleEnabled("ghost-off"))
                p0 = GhostType::Disabled;
        }

        PlayerObject::toggleGhostEffect(p0);
    }
};