#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class Noclip : public Module
{
    public:
        MODULE_SETUP(Noclip)
        {
            setName("Noclip");
            setID("noclip");
            setCategory("Level");
            setDescription("Prevents the player from dying");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

SUBMIT_HACK(Noclip);

class $modify (PlayLayer)
{
    void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        if (p1 == m_anticheatSpike)
            return PlayLayer::destroyPlayer(p0, p1);

        if (!Noclip::get()->getRealEnabled())
            PlayLayer::destroyPlayer(p0, p1);
    }
};