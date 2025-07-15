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
            setDescription("No respawn blnik");
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