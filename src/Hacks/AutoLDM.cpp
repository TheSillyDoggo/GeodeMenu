#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (LevelInfoLayer)
{
    bool init(GJGameLevel* level, bool challenge)
    {
        if (!LevelInfoLayer::init(level, challenge))
            return false;

        m_level->m_lowDetailModeToggled = true;

        return true;
    }

    QOLMOD_MOD_HOOK("auto-ldm", "LevelInfoLayer::init")
};

class $modify (EditLevelLayer)
{
    bool init(GJGameLevel* level)
    {
        if (!EditLevelLayer::init(level))
            return false;

        m_level->m_lowDetailModeToggled = true;

        return true;
    }

    QOLMOD_MOD_HOOK("auto-ldm", "EditLevelLayer::init")
};