#include "../../Client/Module.hpp"
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

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

class $modify (CCLongerStreak, CCMotionStreak)
{
    struct Fields
    {
        bool shouldStreakBeLonger = false;
    };

    virtual void update(float delta)
    {
        if (LongerTrail::get()->getRealEnabled() && m_fields->shouldStreakBeLonger)
            delta /= 3;

        CCMotionStreak::update(delta);
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriorityPost("cocos2d::CCMotionStreak::update", Priority::Last);
    }
};

class $modify (PlayerObject)
{
    bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer)
    {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
            return false;

        as<CCLongerStreak*>(m_regularTrail)->m_fields->shouldStreakBeLonger = true;

        return true;
    }
};