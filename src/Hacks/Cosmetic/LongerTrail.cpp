#include "../../Client/Module.hpp"
#include <Geode/modify/CCMotionStreak.hpp>

using namespace geode::prelude;

class LongerTrail : public Module
{
    public:
        MODULE_SETUP(LongerTrail)
        {
            setName("Longer Trail");
            setID("longer-trail");
            setCategory("Cosmetic");
            setDescription("Longer trail help");
        }
};

SUBMIT_HACK(LongerTrail);

class $modify (CCMotionStreak)
{
    virtual void update(float delta)
    {
        if (LongerTrail::get()->getRealEnabled())
            delta /= 3;

        CCMotionStreak::update(delta);
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriorityPost("cocos2d::CCMotionStreak::update", Priority::Last);
    }
};