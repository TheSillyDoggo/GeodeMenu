#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoRespawnBlink : public Module
{
    public:
        MODULE_SETUP(NoRespawnBlink)
        {
            setName("No Respawn Blink");
            setID("no-blink");
            setCategory("Cosmetic");
            setDescription("No respawn blnik");
        }
};

SUBMIT_HACK(NoRespawnBlink);

class $modify (PlayerObject)
{
    void playSpawnEffect()
    {
        if (NoRespawnBlink::get()->getRealEnabled())
            return;

        PlayerObject::playSpawnEffect();
    }
};