#include "../../Client/Module.hpp"
#include <Geode/modify/CCTransitionFade.hpp>

using namespace geode::prelude;

class NoTransition : public Module
{
    public:
        MODULE_SETUP(NoTransition)
        {
            setName("No Transition");
            setID("no-trans");
            setCategory("Cosmetic");
        }
};

SUBMIT_HACK(NoTransition);

class $modify (CCTransitionFade)
{
    virtual bool initWithDuration(float t, CCScene*scene ,const ccColor3B& color)
    {
        t = 0;

        return CCTransitionFade::initWithDuration(t, scene, color);
    }

    MODULE_ALL_HOOKS(NoTransition);
};