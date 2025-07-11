#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoDeathEffect : public Module
{
    public:
        MODULE_SETUP(NoDeathEffect)
        {
            setName("No Death Effect");
            setID("no-death");
            setCategory("Cosmetic");
            setDescription("Disables the death effect");
        }
};

SUBMIT_HACK(NoDeathEffect);

class $modify (PlayerObject)
{
    void playDeathEffect()
    {
        if (!NoDeathEffect::get()->getRealEnabled())
            PlayerObject::playDeathEffect();
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriorityPost("PlayerObject::playDeathEffect", Priority::Last);
    }
};