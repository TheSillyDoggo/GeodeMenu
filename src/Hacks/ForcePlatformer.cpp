#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS

class $modify (PlayerObject)
{
    virtual void update(float dt)
    {
        if (Client::GetModuleEnabled("force-plat"))
            this->togglePlatformerMode(true);

        PlayerObject::update(dt);
    }
};

#endif