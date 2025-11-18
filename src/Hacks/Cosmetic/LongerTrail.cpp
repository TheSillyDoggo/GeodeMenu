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
            setDescription("Lengthens your player's trail length by a factor of 3x");
        }
};

class LongerTrailPlayerOnly : public Module
{
    public:
        MODULE_SETUP(LongerTrailPlayerOnly)
        {
            setName("Player Only");
            setID("longer-trail/player-only");
            setDescription("Only makes player trails longer, ignoring all other trail effects ingame");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(LongerTrail);
SUBMIT_OPTION(LongerTrail, LongerTrailPlayerOnly);

class $modify (CCLongerStreak, CCMotionStreak)
{
    struct Fields
    {
        bool shouldStreakBeLonger = false;
    };

    virtual void update(float delta)
    {
        if (LongerTrail::get()->getRealEnabled() && (LongerTrailPlayerOnly::get()->getRealEnabled() ? m_fields->shouldStreakBeLonger : true))
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

        static_cast<CCLongerStreak*>(m_regularTrail)->m_fields->shouldStreakBeLonger = true;

        return true;
    }
};