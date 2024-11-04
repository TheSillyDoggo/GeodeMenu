#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (EditorPauseLayer)
{
    void saveLevel()
    {
        if (LevelEditorLayer::get()->m_level->m_levelType == GJLevelType::Editor)
            LevelEditorLayer::get()->m_level->m_normalPercent = 0;

        EditorPauseLayer::saveLevel();
    }

    QOLMOD_MOD_ALL_HOOKS("reset-percentage-on-save")
};