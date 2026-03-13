#include "../../Client/Module.hpp"
#include <Geode/modify/CCTransitionFade.hpp>
#include <Utils.hpp>

using namespace geode::prelude;

class NoTransition : public Module
{
    public:
        MODULE_SETUP(NoTransition)
        {
            setName("No Transition");
            setID("no-trans");
            setCategory("Universal");
            setDescription("Disables the fade transition between scenes");
        }
};

SUBMIT_HACK(NoTransition);

class $modify (ReplacedTransitionFade, CCTransitionFade)
{
    static CCTransitionFade* create(float duration,CCScene* scene)
    {
        if (NoTransition::get()->getRealEnabled())
            duration = 0;

        return static_cast<CCTransitionFade*>(qolmod::utils::createTransitionForEnum(kCCTransitionCrossFade, duration, scene));
    }
};