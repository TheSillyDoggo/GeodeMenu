/*#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PauseLayer)
{
    void onPrac(float dt)
    {
        log::info("alert clicked");
    }

    void onPracticeMode(cocos2d::CCObject* sender)
    {
        if (PlayLayer::get()->m_isPracticeMode)
            { PlayLayer::get()->resetLevelFromStart(); PlayLayer::get()->togglePracticeMode(false); PlayLayer::get()->pauseGame(false); return; }
        

        geode::createQuickPopup(
            "Practice Mode",
            "Are you sure you want to\nenter <cg>practice mode</c>?",
            "Cancel", "Practice",
            [](FLAlertLayer* tis, bool btn2) {
                log::info("click practice");

                if (btn2) {
                    log::info("right btn");

                    PlayLayer::get()->togglePracticeMode(true);
                    PlayLayer::get()->pauseGame(false);
                }
            }
        );
    }
};*/