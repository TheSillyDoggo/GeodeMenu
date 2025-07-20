#include "../../Client/Module.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class NoMirrorPortal : public Module
{
    public:
        MODULE_SETUP(NoMirrorPortal)
        {
            setName("No Mirror Portal");
            setID("no-reverse");
            setCategory("Level");
            setDescription("Disables mirror portals");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

SUBMIT_HACK(NoMirrorPortal);

class $modify (GJBaseGameLayer)
{
    void toggleFlipped(bool p0, bool p1)
    {
        if (NoMirrorPortal::get()->getRealEnabled())
            return;

        GJBaseGameLayer::toggleFlipped(p0, p1);
    }
};