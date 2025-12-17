#include "../../Client/InputModule.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class StopTriggersOnDeath : public Module
{
    public:
        MODULE_SETUP(StopTriggersOnDeath)
        {
            setName("Stop Triggers on Death");
            setID("stop-triggers-on-death");
            setCategory("Level");
            setDescription("Stops triggers whenever you die");
        }
};

SUBMIT_HACK(StopTriggersOnDeath);

class $modify (GJBaseGameLayer)
{
    virtual void update(float dt)
    {
        if (StopTriggersOnDeath::get()->getRealEnabled())
        {
            if (m_player1 && m_player1->m_isDead)
                return;

            if (m_player2 && m_player2->m_isDead)
                return;
        }

        GJBaseGameLayer::update(dt);
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriority("GJBaseGameLayer::update", Priority::Last);
    }
};