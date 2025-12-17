#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class ForcePlatformer : public Module
{
    public:
        MODULE_SETUP(ForcePlatformer)
        {
            setName("Force Platformer");
            setID("force-plat");
            setCategory("Level");
            setDescription("Force Platformer mode on all levels.\nReopen level to apply.");
            setSafeModeTrigger(SafeModeTrigger::LevelLoad);
        }
};

SUBMIT_HACK(ForcePlatformer);

class $modify (PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        if (ForcePlatformer::get()->getRealEnabled())
        {
            m_player1->togglePlatformerMode(true);

            if (m_player2)
                m_player2->togglePlatformerMode(true);

            #ifdef GEODE_IS_MOBILE

            if (m_uiLayer)
                m_uiLayer->togglePlatformerMode(true);

            #endif
        }

        return true;
    }
};