#include "../../Client/Module.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class ConfirmPractice : public Module
{
    public:
        MODULE_SETUP(ConfirmPractice)
        {
            setName("Confirm Practice");
            setID("conf-prac");
            setCategory("Level");
            setDescription("Adds a popup to confirm if you want to enter practice mode");
        }
};

class ConfirmRestart : public Module
{
    public:
        MODULE_SETUP(ConfirmRestart)
        {
            setName("Confirm Restart");
            setID("conf-res");
            setCategory("Level");
            setDescription("Adds a popup to confirm if you want to restart the level");
        }
};

SUBMIT_HACK(ConfirmPractice);
SUBMIT_HACK(ConfirmRestart);

class $modify (PauseLayer)
{
    struct Fields {
        bool v = false;
        bool a = false;
    };

    void onNormalMode(cocos2d::CCObject* sender)
    {
        if (m_fields->v || !ConfirmPractice::get()->getRealEnabled())
        {
            PauseLayer::onNormalMode(sender);

            return;
        }        

        geode::createQuickPopup(
            "Practice Mode",
            "Are you sure you want to\n<cr>exit</c> <cg>Practice Mode</c>?",
            "Cancel", "Exit",
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
        if (m_fields->v || !ConfirmPractice::get()->getRealEnabled())
        {
            PauseLayer::onPracticeMode(sender);

            return;
        }        

        geode::createQuickPopup(
            "Practice Mode",
            "Are you sure you want to\n<cy>enter</c> <cg>Practice Mode</c>?",
            "Cancel", "Practice",
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
        if (m_fields->a || !ConfirmRestart::get()->getRealEnabled())
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
        if (m_fields->a || !ConfirmRestart::get()->getRealEnabled())
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