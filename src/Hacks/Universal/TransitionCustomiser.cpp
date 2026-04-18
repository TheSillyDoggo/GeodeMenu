#include "../../Client/Module.hpp"
#include "../../Client/EnumModule.hpp"
#include "../../Client/InputModule.hpp"
#include <Geode/modify/CCTransitionFade.hpp>
#include <Geode/modify/CCGridBase.hpp>
#include <Utils.hpp>

using namespace geode::prelude;

class NoTransition : public Module
{
    public:
        MODULE_SETUP(NoTransition)
        {
            setID("no-trans");
            setCategory("Universal");

            setDisabled(true);
            setDisabledMessage("Transition customizer has been temporarily disabled due to crashing when getting achievements");
        }
};

class TransititionCustomiser : public Module
{
    public:
        MODULE_SETUP(TransititionCustomiser)
        {
            setID("transition-customiser");
            setCategory("Universal");
            setPriority(6);

            setDisabled(true);
            setDisabledMessage("Transition customizer has been temporarily disabled due to crashing when getting achievements");

            /*
            #if GEODE_COMP_GD_VERSION == 22074
            #ifdef __APPLE__
            setDisabled(true);
            setDisabledMessage("Unsupported on 2.2074");
            #endif
            #endif
            */
        }
};

class TransitionCustomiserTransition : public EnumModule
{
    public:
        MODULE_SETUP(TransitionCustomiserTransition)
        {
            setID("transition-customiser/transition");
            setDefaultValue(kCCTransitionFade);
            initValues<kCCTransition>();
            setPriority(1);
        }
};

class TransitionDuration : public InputModule
{
    public:
        MODULE_SETUP(TransitionDuration)
        {
            setID("transition-customiser/duration");
            setPriority(2);

            setDefaultString("0.50");
            setHint("seconds");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

SUBMIT_HACK(NoTransition);
SUBMIT_HACK(TransititionCustomiser);
SUBMIT_OPTION(TransititionCustomiser, TransitionCustomiserTransition);
SUBMIT_OPTION(TransititionCustomiser, TransitionDuration);

/*
class $modify (ReplacedTransitionFade, CCTransitionFade)
{
    static CCTransitionFade* create(float duration, CCScene* scene)
    {
        if (NoTransition::get()->getRealEnabled())
            duration = 0;

        auto type = kCCTransitionFade;

        if (TransititionCustomiser::get()->getRealEnabled())
        {
            type = (kCCTransition)TransitionCustomiserTransition::get()->getValue();
            duration = TransitionDuration::get()->getStringFloat();
        }

        auto ret = static_cast<CCTransitionFade*>(qolmod::utils::createTransitionForEnum(type, duration, scene));

        if (!(typeinfo_cast<CCTransitionFade*>(ret) || typeinfo_cast<CCTransitionCrossFade*>(ret) || typeinfo_cast<CCTransitionMoveInL*>(ret)))
            CCDirector::get()->willSwitchToScene(ret->m_pOutScene);

        return ret;
    }
};*/