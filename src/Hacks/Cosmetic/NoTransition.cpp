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
            setDescription("Disables the fade transition between scenes");
        }
};

SUBMIT_HACK(NoTransition);

class $modify (CCTransitionFade)
{
    virtual bool initWithDuration(float t, CCScene*scene ,const ccColor3B& color)
    {
        return CCTransitionFade::initWithDuration(NoTransition::get()->getRealEnabled() ? 0 : t, scene, color);
    }
};