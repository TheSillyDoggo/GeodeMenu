#include "Uncomplete.hpp"

using namespace geode::prelude;

void Uncomplete::FLAlert_Clicked(FLAlertLayer* p0, bool p1) {
    if (p1)
    {
        log::info("uncomplete");

        GJGameLevel* level = nullptr;

        if (PlayLayer::get())
            level = PlayLayer::get()->m_level;

        if (getChildOfType<LevelInfoLayer>(CCScene::get(), 0))
            level = getChildOfType<LevelInfoLayer>(CCScene::get(), 0)->m_level;

        if (!level)
            return;

        level->m_normalPercent = 0;
        level->m_practicePercent = 0;
        level->m_attempts = 0;
        level->m_jumps = 0;
    }
}

void Uncomplete::onToggle(CCObject* sender) {
    auto btn = as<CCMenuItemToggler*>(sender);

    log::info("dont show: {}", !btn->isToggled());

    Mod::get()->setSavedValue<bool>("uncomplete-dont-show", !btn->isToggled());
}