#include "../../Client/KeybindModule.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../../GUI/StartposSwitcherUI.hpp"
#include "../Utils/PlayLayer.hpp"

using namespace geode::prelude;

class StartposSwitcher : public Module
{
    public:
        MODULE_SETUP(StartposSwitcher)
        {
            setName("Startpos Switcher");
            setID("startpos-switcher");
            setDefaultEnabled(true);
            setCategory("Level");
            setDescription("Switch between start positions in the level at any time");
            setPriority(5);
        }
};

class StartposSwitcherPrev : public KeybindModule
{
    public:
        MODULE_SETUP(StartposSwitcherPrev)
        {
            setName("Prev:");
            setID("startpos-switcher/prev-bind");
            setDescription("");
            setPriority(1);

            setDefaultKeyCode(81);
        }
};

class StartposSwitcherNext : public KeybindModule
{
    public:
        MODULE_SETUP(StartposSwitcherNext)
        {
            setName("Next:");
            setID("startpos-switcher/next-bind");
            setDescription("");
            setPriority(2);

            setDefaultKeyCode(69); // hehe
        }
};

class StartposSwitcherHide : public Module
{
    public:
        MODULE_SETUP(StartposSwitcherHide)
        {
            setName("Hide UI");
            setID("startpos-switcher/hide-ui");
            setDescription("Hides the UI and makes it non-interactable");
            setPriority(3);
        }
};

SUBMIT_HACK(StartposSwitcher);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherPrev);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherNext);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherHide);

class $modify (StartposPlayLayer, PlayLayer)
{
    struct Fields
    {
        StartposSwitcherUI* ui = nullptr;
        int startPosIndex = 0;
    };

    void createObjectsFromSetupFinished()
    {
        PlayLayer::createObjectsFromSetupFinished();

        m_fields->ui = StartposSwitcherUI::create();
        m_fields->ui->setPosition(ccp(CCDirector::get()->getWinSize().width / 2, 35));

        m_fields->ui->onSwitchFunc = [this](int switchBy)
        {
            this->switchStartPosByAmount(switchBy);
        };

        if (m_uiLayer)
            m_uiLayer->addChild(m_fields->ui);

        if (StartposSwitcher::get()->getRealEnabled())
            setStartPos(0);
    }

    void switchStartPosByAmount(int amount)
    {
        setStartPos(m_fields->startPosIndex + amount);
    }

    void setStartPos(int index)
    {
        if (!PlayLayerUtils::getUtils())
            return;

        if (PlayLayerUtils::getUtils()->m_fields->startPositions.empty())
            return;

        std::sort(PlayLayerUtils::getUtils()->m_fields->startPositions.begin(), PlayLayerUtils::getUtils()->m_fields->startPositions.end(), [](auto* a, auto* b) { return a->getPositionX() < b->getPositionX(); });

        if (index < 0)
            index = PlayLayerUtils::getUtils()->m_fields->startPositions.size();

        if (index > PlayLayerUtils::getUtils()->m_fields->startPositions.size())
            index = 0;

        m_fields->startPosIndex = index;

        m_currentCheckpoint = nullptr;

        auto spos = index == 0 ? nullptr : PlayLayerUtils::getUtils()->m_fields->startPositions[m_fields->startPosIndex - 1];

        setStartPosObject(spos);

        if (isRunning())
        {
            if (m_isPracticeMode)
                resetLevelFromStart();

            resetLevel();
            startMusic();
        }

        m_fields->ui->setStartposVisual(m_fields->startPosIndex);
    }

    void postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        if (m_fields->ui)
        {
            m_fields->ui->setVisible(StartposSwitcher::get()->getRealEnabled() && !StartposSwitcherHide::get()->getRealEnabled());
            m_fields->ui->updateKeybindsVisualizer(false, StartposSwitcherPrev::get()->getKeyCode(), StartposSwitcherNext::get()->getKeyCode());
        }
    }
};

class $modify (StartposUILayer, UILayer)
{
    void handleKeypress(cocos2d::enumKeyCodes key, bool down)
    {
        if (down && StartposSwitcher::get()->getRealEnabled())
        {
            if (auto pl = static_cast<StartposPlayLayer*>(PlayLayer::get()))
            {
                if (key == StartposSwitcherPrev::get()->getKeyCode())
                    pl->switchStartPosByAmount(-1);

                if (key == StartposSwitcherNext::get()->getKeyCode())
                    pl->switchStartPosByAmount(1);
            }
        }

        UILayer::handleKeypress(key, down);
    }
};