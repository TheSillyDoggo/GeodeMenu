#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

class $modify (PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        m_levelSettings->m_fixGravityBug = false;

        return true;
    }

    QOLMOD_MOD_HOOK("legacy-upside-down", "PlayLayer::init")
};