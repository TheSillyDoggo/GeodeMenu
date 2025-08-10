#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class InstantComplete : public Module
{
    public:
        MODULE_SETUP(InstantComplete)
        {
            setName("Instant Complete");
            setID("instant");
            setCategory("Level");
            setDescription("Instantly Completes a level.");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
            setPriority(2);
        }
};

SUBMIT_HACK(InstantComplete);

class $modify (PlayLayer)
{
    struct Fields
    {
        bool done = false;
    };

    virtual void postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        if (InstantComplete::get()->getRealEnabled())
        {
            if (m_player1->isRunning() && !m_fields->done)
            {
                playPlatformerEndAnimationToPos(ccp(2, 2), false);
                m_fields->done = true;
            }
        }
    }
};