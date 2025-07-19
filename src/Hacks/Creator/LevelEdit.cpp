#include "../../Client/Module.hpp"
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class LevelEdit : public Module
{
    public:
        MODULE_SETUP(LevelEdit)
        {
            setName("Level Edit");
            setID("level-edit");
            setCategory("Creator");
            setDescription("Allows you to edit any level");
        }
};

SUBMIT_HACK(LevelEdit);

class $modify (PauseLayer)
{
    virtual void customSetup()
    {
        auto level = GJBaseGameLayer::get()->m_level;
        auto lType = level->m_levelType;

        if (LevelEdit::get()->getRealEnabled())
            level->m_levelType = GJLevelType::Editor;

        PauseLayer::customSetup();

        level->m_levelType = lType;
    }

    void onTryEdit(cocos2d::CCObject* sender)
    {
        if (LevelEdit::get()->getRealEnabled())
            return PauseLayer::goEdit();

        PauseLayer::onTryEdit(sender);
    }
};

class $modify (EditorPauseLayer)
{
    virtual void customSetup()
    {
        auto level = GJBaseGameLayer::get()->m_level;
        auto lType = level->m_levelType;

        if (LevelEdit::get()->getRealEnabled())
            level->m_levelType = GJLevelType::Editor;

        EditorPauseLayer::customSetup();

        level->m_levelType = lType;
    }
};