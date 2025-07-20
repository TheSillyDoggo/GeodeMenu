#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class NoSpiderDash : public Module
{
    public:
        MODULE_SETUP(NoSpiderDash)
        {
            setName("No Spider Dash Effect");
            setID("no-spider-dash");
            setCategory("Cosmetic");
            setDescription("Hides the spider dash effect when you teleport");
        }
};

SUBMIT_HACK(NoSpiderDash);

class $modify (PlayerObject)
{
    void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to)
    {
        if (NoSpiderDash::get()->getRealEnabled())
            return;

        PlayerObject::playSpiderDashEffect(from, to);
    }
};