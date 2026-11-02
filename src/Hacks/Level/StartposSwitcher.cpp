#include "../../Client/KeybindModule.hpp"
#include "../../Client/LayoutModule.hpp"
#include "../../Client/FloatSliderModule.hpp"
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
            setID("startpos-switcher");
            setDefaultEnabled(true);
            setCategory("Level");
            setPriority(5);
        }
};

class StartposSwitcherLayout : public LayoutModule
{
    public:
        MODULE_SETUP(StartposSwitcherLayout)
        {
            setID("startpos-switcher/layout");
            setPriority(1);
            setDefaults(Anchor::Bottom, ccp(0, 35));
            
            addOption("scale", "scale", 1.0f, 0.0f, 1.0f);
            addOption("opacity", "opacity", 0.8f, 0.0f, 1.0f);

            setPreviewNode([]{
                auto node = StartposSwitcherUI::create(nullptr, false);
                node->regularOpacity = 0.5f;
                return node;
            });
        }
};

class StartposSwitcherPercentage : public Module
{
    public:
        MODULE_SETUP(StartposSwitcherPercentage)
        {
            setID("startpos-switcher/use-percentage");
            setPriority(2);
        }
};

class StartposSwitcherShowName : public Module
{
    public:
        MODULE_SETUP(StartposSwitcherShowName)
        {
            setID("startpos-switcher/show-name");
            setDefaultEnabled(true);
            setPriority(3);
        }
};

class StartposSwitcherOpacity : public FloatSliderModule
{
    public:
        MODULE_SETUP(StartposSwitcherOpacity)
        {
            setID("startpos-switcher/opacity");
            setDefaultValue(0.2f);
            setSnapValues({ 0.2f });
            setRange(0, 1.0f);
            setPriority(4);
        }
};

SUBMIT_HACK(StartposSwitcher);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherLayout);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherPercentage);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherShowName);
SUBMIT_OPTION(StartposSwitcher, StartposSwitcherOpacity);

class $modify (StartposPlayLayer, PlayLayer)
{
    struct Fields
    {
        StartposSwitcherUI* ui = nullptr;
    };

    void createObjectsFromSetupFinished()
    {
        PlayLayer::createObjectsFromSetupFinished();

        m_fields->ui = StartposSwitcherUI::create(this);
        StartposSwitcherLayout::get()->applyToNode(m_fields->ui);

        if (m_uiLayer)
            m_uiLayer->addChild(m_fields->ui);
    }

    void postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        if (m_fields->ui)
        {
            m_fields->ui->setVisible(StartposSwitcher::get()->getRealEnabled());
            m_fields->ui->usePercentage = StartposSwitcherPercentage::get()->getRealEnabled();
            m_fields->ui->showName = StartposSwitcherShowName::get()->getRealEnabled();
            m_fields->ui->regularOpacity = StartposSwitcherOpacity::get()->getValue();
            m_fields->ui->updateUI();
        }
    }
};