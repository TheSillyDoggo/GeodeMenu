#include "../../Client/Module.hpp"
#include <Geode/modify/CCCircleWave.hpp>

using namespace geode::prelude;

class NoCircles : public Module
{
    public:
        MODULE_SETUP(NoCircles)
        {
            setName("No Circles");
            setID("no-circles");
            setCategory("Cosmetic");
            setDescription("No circles");
        }
};

SUBMIT_HACK(NoCircles);

class $modify (CCCircleWave)
{
    virtual void draw()
    {
        if (NoCircles::get()->getRealEnabled() && PlayLayer::get())
            return;

        CCCircleWave::draw();
    }
};