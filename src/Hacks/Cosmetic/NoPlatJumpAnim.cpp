#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoPlatJumpAnim : public Module
{
    public:
        MODULE_SETUP(NoPlatJumpAnim)
        {
            setName("No Plat Jump Anim");
            setID("plat-jump-anim");
            setCategory("Cosmetic");
            setDescription("No particle help");
        }
};

SUBMIT_HACK(NoPlatJumpAnim);

class $modify (PlayerObject)
{
    void animatePlatformerJump(float p0)
    {
        if (NoPlatJumpAnim::get()->getRealEnabled())
            return;

        PlayerObject::animatePlatformerJump(p0);
    }
};