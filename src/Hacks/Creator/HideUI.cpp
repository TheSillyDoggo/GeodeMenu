#include "../../Client/Module.hpp"
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class HideEditorUI : public Module
{
    public:
        MODULE_SETUP(HideEditorUI)
        {
            setID("hide-editor-ui");
            setCategory("Creator");
        }
};

SUBMIT_HACK(HideEditorUI);

class $modify (HideEditorUIHook, EditorUI)
{
    void updateVisibility(float dt)
    {
        this->setVisible(!HideEditorUI::get()->getRealEnabled());
    }

    bool init(LevelEditorLayer* editorLayer)
    {
        if (!EditorUI::init(editorLayer))
            return false;

        this->schedule(schedule_selector(HideEditorUIHook::updateVisibility));

        return true;
    }
};