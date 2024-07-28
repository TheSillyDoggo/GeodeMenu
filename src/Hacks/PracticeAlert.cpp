#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PauseLayer)
{
    struct Fields {
        bool v = false;
        bool a = false;
    };

    void onNormalMode(cocos2d::CCObject* sender)
    {
        if (m_fields->v || !Client::GetModuleEnabled("conf-prac"))
        {
            PauseLayer::onNormalMode(sender);

            return;
        }        

        geode::createQuickPopup(
            "Practice Mode",
            "Are you sure you want to\n<cr>exit</c> <cg>Practice Mode</c>?",
            "Cancel", "Practice",
            [this, sender](FLAlertLayer* tis, bool btn2) {
                if (btn2)
                {
                    this->m_fields->v = true;

                    this->onNormalMode(sender);

                    this->m_fields->v = false;
                }
            }
        );
    }

    void onPracticeMode(cocos2d::CCObject* sender)
    {
        if (m_fields->v || !Client::GetModuleEnabled("conf-prac"))
        {
            PauseLayer::onPracticeMode(sender);

            return;
        }        

        geode::createQuickPopup(
            "Practice Mode",
            "Are you sure you want to\n<cy>enter</c> <cg>Practice Mode</c>?",
            "Cancel", "Exit",
            [this, sender](FLAlertLayer* tis, bool btn2) {
                if (btn2)
                {
                    this->m_fields->v = true;

                    this->onPracticeMode(sender);

                    this->m_fields->v = false;
                }
            }
        );
    }

    void onRestart(cocos2d::CCObject* sender)
    {
        if (m_fields->a || !Client::GetModuleEnabled("conf-res"))
        {
            PauseLayer::onRestart(sender);

            return;
        }        

        geode::createQuickPopup(
            "Restart Level",
            "Are you sure you want to\n<cr>restart this level</c>?",
            "Cancel", "Restart",
            [this, sender](FLAlertLayer* tis, bool btn2) {
                if (btn2) {
                    this->m_fields->a = true;

                    this->onRestart(sender);

                    this->m_fields->a = false;
                }
            }
        );
    }

    void onRestartFull(cocos2d::CCObject* sender)
    {
        if (m_fields->a || !Client::GetModuleEnabled("conf-res"))
        {
            PauseLayer::onRestartFull(sender);

            return;
        }        

        geode::createQuickPopup(
            "Restart Leevl",
            "Are you sure you want to\n<cr>restart this level</c>?",
            "Cancel", "Restart",
            [this, sender](FLAlertLayer* tis, bool btn2) {
                if (btn2) {
                    this->m_fields->a = true;

                    this->onRestartFull(sender);

                    this->m_fields->a = false;
                }
            }
        );
    }
};