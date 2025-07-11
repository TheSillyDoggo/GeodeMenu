#include "../Client/Module.hpp"
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
        }
};

SUBMIT_HACK(Noclip);

class $modify (PlayLayer)
{
    void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        if (p1 == m_anticheatSpike)
            PlayLayer::destroyPlayer(p0, p1);
    }

    MODULE_ALL_HOOKS(Noclip);
};