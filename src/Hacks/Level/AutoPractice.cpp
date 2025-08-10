#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include "../Utils/PlayLayer.hpp"

using namespace geode::prelude;

class AutoPracticeMode : public Module
{
    public:
        MODULE_SETUP(AutoPracticeMode)
        {
            setName("Auto Practice Mode");
            setID("auto-practice");
            setCategory("Level");
            setDescription("Automatically puts you into practice mode when you open a level");
        }
};

class AutoPracticeDemonsOnly : public Module
{
    public:
        MODULE_SETUP(AutoPracticeDemonsOnly)
        {
            setName("Demons Only");
            setID("auto-practice/demons-only");
            setCategory("Level");
            setDescription("Only puts you into practice mode if the level is a demon");
        }
};

SUBMIT_HACK(AutoPracticeMode);
SUBMIT_OPTION(AutoPracticeMode, AutoPracticeDemonsOnly);

class $modify (PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        if (AutoPracticeMode::get()->getRealEnabled() ? (AutoPracticeDemonsOnly::get()->getRealEnabled() ? (m_level->m_difficulty >= GJDifficulty::Demon) : true) : false)
            togglePracticeMode(true);

        return true;
    }
};