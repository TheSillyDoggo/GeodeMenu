/*#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (AlertPauseLayer, PauseLayer)
{
    void onPrac(float dt)
    {
        log::info("alert clicked");
    }

    void onPracticeMode(cocos2d::CCObject* sender)
    {
        //create(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4)
        auto al = FLAlertLayer::create(this, "");

        al->show();
        geode::createQuickPopup(
            "Practice Mode",
            "Are you sure you want to enter\n<cg>practice mode</c>",
            "Cancel", "Practice",
            [](auto tis, bool btn2) {
                if (btn2) {
                    tis->scheduleOnce(schedule_selector(AlertPauseLayer::onPrac), 1.0f);
                }
            }
        );
    }
};*/