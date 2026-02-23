#include "../../Client/Module.hpp"
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class ResetPercentageOnSave : public Module
{
    public:
        MODULE_SETUP(ResetPercentageOnSave)
        {
            setID("reset-percentage-on-save");
            setCategory("Creator");
        }
};

SUBMIT_HACK(ResetPercentageOnSave);

class $modify (EditorPauseLayer)
{
    void saveLevel()
    {
        if (ResetPercentageOnSave::get()->getRealEnabled())
        {
            if (LevelEditorLayer::get()->m_level->m_levelType == GJLevelType::Editor)
                LevelEditorLayer::get()->m_level->m_normalPercent = 0;
        }

        EditorPauseLayer::saveLevel();
    }
};