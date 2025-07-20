#include "../../Client/Module.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class InstantMirrorPortal : public Module
{
    public:
        MODULE_SETUP(InstantMirrorPortal)
        {
            setName("Instant Mirror Portal");
            setID("instant-reverse");
            setCategory("Level");
            setDescription("Makes the mirror portal transition instant");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

SUBMIT_HACK(InstantMirrorPortal);

class $modify (GJBaseGameLayer)
{
    void toggleFlipped(bool p0, bool p1)
    {
        if (InstantMirrorPortal::get()->getRealEnabled())
            p1 = true;

        GJBaseGameLayer::toggleFlipped(p0, p1);
    }
};