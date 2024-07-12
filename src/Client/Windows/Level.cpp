#include "Level.h"
#include "Uncomplete.h"
#include "../../Layers/ConfirmFLAlertLayer.h"

using namespace geode::prelude;

void Level::onFix(CCObject*)
{
    if (PlayLayer::get())
    {
        PlayLayer::get()->m_level->m_levelType = GJLevelType::Saved;

        // fucking disgusting
        if ((PlayLayer::get()->m_level->m_levelID > 0 && PlayLayer::get()->m_level->m_levelID < 127) || PlayLayer::get()->m_level->m_levelID == 3001 || PlayLayer::get()->m_level->m_levelID == 5001 || PlayLayer::get()->m_level->m_levelID == 5002 || PlayLayer::get()->m_level->m_levelID == 5003 || PlayLayer::get()->m_level->m_levelID == 5004)
        {
            PlayLayer::get()->m_level->m_levelType = GJLevelType::Local;
        }

        if (PlayLayer::get()->m_level->m_levelID == 0)
            PlayLayer::get()->m_level->m_levelType = GJLevelType::Editor;
    }
}

void Level::onUnc(CCObject* sender) {
    if (!(PlayLayer::get() || getChildOfType<LevelInfoLayer>(CCScene::get(), 0)))
        return FLAlertLayer::create("Uncomplete Level", "You must be in a level or on a level page to uncomplete a level", "OK")->show();
    
    auto protocol = new Uncomplete();

    if (Mod::get()->getSavedValue<bool>("uncomplete-dont-show", false) && !CCKeyboardDispatcher::get()->getShiftKeyPressed())
    {
        protocol->FLAlert_Clicked(nullptr, true);
    }
    else
    {
        auto alert = ConfirmFLAlertLayer::create(protocol, "Uncomplete Level", "Don't show this popup again", false, menu_selector(Uncomplete::onToggle), "Are you sure you want to <cr>uncomplete</c> this <cg>level</c>? Uncompleting a level will:\n<cl>- Reset Attempts</c>\n<cl>- Reset Percentage</c>\n<cl>- Reset Jumps</c>", "Cancel", "Reset", 350, false, 310, 1.0f);
        alert->show();
    }
}