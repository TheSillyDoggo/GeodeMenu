#include "../../Client/KeybindModule.hpp"
#include "../../Client/LayoutModule.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../../GUI/StartposSwitcherUI.hpp"
#include "../Utils/PlayLayer.hpp"
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <cstring>

using namespace geode::prelude;

namespace
{
    bool suppressStartposAttempt = false;

    struct StartposAttemptGuard
    {
        bool previous;

        explicit StartposAttemptGuard(bool suppress)
          : previous(suppressStartposAttempt)
        {
            suppressStartposAttempt = previous || suppress;
        }

        ~StartposAttemptGuard()
        {
            suppressStartposAttempt = previous;
        }
    };
}

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

class StartposSwitcherPercentage : public Module
{
    public:
        MODULE_SETUP(StartposSwitcherPercentage)
        {
            setName("Use Percentage");
            setID("startpos-switcher/use-percentage");
            setDescription("Shows the percentage that each start position is at");
            setPriority(4);
        }
};

class StartposSwitcherLayout : public LayoutModule
{
    public:
        MODULE_SETUP(StartposSwitcherLayout)
        {
            setID("startpos-switcher/layout");
            setPriority(5);
            setDefaults(Anchor::Bottom, ccp(0, 35));
            
            addOption("scale", "scale", 1.0f, 0.0f, 1.0f);
            addOption("opacity", "opacity", 0.8f, 0.0f, 1.0f);

            setPreviewNode([]{
                return StartposSwitcherUI::create(false);
            });
        }
};

SUBMIT_HACK(StartposSwitcher);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherPrev);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherNext);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherHide);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherPercentage);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherLayout);

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
        StartposSwitcherLayout::get()->applyToNode(m_fields->ui);

        m_fields->ui->onSwitchFunc = [this](int switchBy)
        {
            this->switchStartPosByAmount(switchBy);
        };

        if (m_uiLayer)
            m_uiLayer->addChild(m_fields->ui);

        if (StartposSwitcher::get()->getRealEnabled())
            setStartPos(0);
    }

    int getSelectedStartPos()
    {
        return m_fields->startPosIndex;
    }

    void switchStartPosByAmount(int amount)
    {
        setStartPos(m_fields->startPosIndex + amount);
    }

    void setStartPos(int index)
    {
        auto utils = PlayLayerUtils::getUtils();

        if (!utils || utils->m_fields->startPositions.empty())
            return;

        int count = static_cast<int>(
            utils->m_fields->startPositions.size()
        );

        if (index < 0)
            index = count;
        else if (index > count)
            index = 0;

        m_fields->startPosIndex = index;

        if (m_fields->ui)
            m_fields->ui->setStartposVisual(index);

        // Reset immediately so the camera previews the selected position in pause.
        applySelectedStartPos(m_isPaused);
    }

    void applySelectedStartPos(bool preserveAttempts)
    {
        auto utils = PlayLayerUtils::getUtils();
        if (!utils)
            return;

        auto const& positions = utils->m_fields->startPositions;
        int index = m_fields->startPosIndex;

        if (index < 0 || index > static_cast<int>(positions.size()))
            return;

        m_currentCheckpoint = nullptr;
        setStartPosObject(index == 0 ? nullptr : positions[index - 1]);

        // GD calls onExit when pausing, so isRunning() is false in pause too.
        // Only skip the reset during initial setup, before the layer is active.
        if (!isRunning() && !m_isPaused)
            return;

        bool wasPaused = m_isPaused;
        auto sessionAttempts = m_attempts;
        auto levelAttempts = m_level->m_attempts;
        std::string attemptText;
        if (preserveAttempts && m_attemptLabel)
            attemptText = m_attemptLabel->getString();

        {
            StartposAttemptGuard guard(preserveAttempts);

            if (m_isPracticeMode)
                resetLevelFromStart();

            PlayLayer::resetLevel();
        }

        if (preserveAttempts)
        {
            m_attempts = sessionAttempts;
            m_level->m_attempts = levelAttempts;
            if (m_attemptLabel)
                m_attemptLabel->setString(attemptText.c_str());
        }

        startMusic();

        if (wasPaused)
        {
            // resetLevel updates the camera without closing the existing pause menu.
            m_isPaused = true;
            pauseAudio();
        }
    }

    void postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        if (m_fields->ui)
        {
            m_fields->ui->setVisible(StartposSwitcher::get()->getRealEnabled() && !StartposSwitcherHide::get()->getRealEnabled());
            m_fields->ui->updateKeybindsVisualizer(false, StartposSwitcherPrev::get()->getKeyCode(), StartposSwitcherNext::get()->getKeyCode());
            m_fields->ui->usePercentage = StartposSwitcherPercentage::get()->getRealEnabled();
            m_fields->ui->updateUI();
        }
    }
};

class $modify (StartposUILayer, UILayer)
{
    #if GEODE_COMP_GD_VERSION >= 22081
    void handleKeypress(cocos2d::enumKeyCodes key, bool down, double timestamp)
    #else
    void handleKeypress(cocos2d::enumKeyCodes key, bool down)
    #endif
    {
        if (down && StartposSwitcher::get()->getRealEnabled())
        {
            if (auto pl = static_cast<StartposPlayLayer*>(PlayLayer::get()))
            {
                if (!pl->m_levelEndAnimationStarted)
                {
                    if (key == StartposSwitcherPrev::get()->getKeyCode())
                        pl->switchStartPosByAmount(-1);

                    if (key == StartposSwitcherNext::get()->getKeyCode())
                        pl->switchStartPosByAmount(1);
                }
            }
        }

        #if GEODE_COMP_GD_VERSION >= 22081
        UILayer::handleKeypress(key, down, timestamp);
        #else
        UILayer::handleKeypress(key, down);
        #endif
    }
};

class $modify (StartposPauseLayer, PauseLayer)
{
    struct Fields
    {
        StartposSwitcherUI* ui = nullptr;
    };

    void customSetup()
    {
        PauseLayer::customSetup();

        auto pl = static_cast<StartposPlayLayer*>(PlayLayer::get());
        auto utils = PlayLayerUtils::getUtils();
        if (!pl || !utils || utils->m_fields->startPositions.empty() ||
            !StartposSwitcher::get()->getRealEnabled())
            return;

        m_fields->ui = StartposSwitcherUI::create();
        StartposSwitcherLayout::get()->applyToNode(m_fields->ui);
        m_fields->ui->usePercentage = StartposSwitcherPercentage::get()->getRealEnabled();
        m_fields->ui->setStartposVisual(pl->getSelectedStartPos());
        m_fields->ui->setVisible(!StartposSwitcherHide::get()->getRealEnabled());
        m_fields->ui->onSwitchFunc = [this](int amount)
        {
            switchStartPosByAmount(amount);
        };
        addChild(m_fields->ui, 100);
    }

    void switchStartPosByAmount(int amount)
    {
        auto pl = static_cast<StartposPlayLayer*>(PlayLayer::get());
        if (!pl || !pl->m_isPaused || pl->m_levelEndAnimationStarted ||
            !StartposSwitcher::get()->getRealEnabled())
            return;

        pl->switchStartPosByAmount(amount);
        if (m_fields->ui)
            m_fields->ui->setStartposVisual(pl->getSelectedStartPos());
    }

    #if GEODE_COMP_GD_VERSION >= 22081
    void keyDown(cocos2d::enumKeyCodes key, double timestamp)
    #else
    void keyDown(cocos2d::enumKeyCodes key)
    #endif
    {
        if (StartposSwitcher::get()->getRealEnabled())
        {
            if (key == StartposSwitcherPrev::get()->getKeyCode())
            {
                switchStartPosByAmount(-1);
                return;
            }
            if (key == StartposSwitcherNext::get()->getKeyCode())
            {
                switchStartPosByAmount(1);
                return;
            }
        }

        #if GEODE_COMP_GD_VERSION >= 22081
        PauseLayer::keyDown(key, timestamp);
        #else
        PauseLayer::keyDown(key);
        #endif
    }
};

class $modify (StartposAttemptStats, GameStatsManager)
{
    void incrementStat(char const* key, int amount)
    {
        // Stat 2 is the global attempt count, also used by FreezeAttempts.
        if (suppressStartposAttempt && std::strcmp(key, "2") == 0)
            return;

        GameStatsManager::incrementStat(key, amount);
    }
};
