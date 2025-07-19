#include "../../Client/Module.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/ShareLevelLayer.hpp>

using namespace geode::prelude;

class VerifyHack : public Module
{
    public:
        MODULE_SETUP(VerifyHack)
        {
            setName("Verify Hack");
            setID("verify-hack");
            setCategory("Creator");
            setDescription("a");
        }
};

SUBMIT_HACK(VerifyHack);

class $modify (ShareLevelLayer)
{
    struct Fields
    {
        GJGameLevel* level;
    };

    bool init(GJGameLevel* level)
    {
        if (!ShareLevelLayer::init(level))
            return false;

        m_fields->level = level;

        return true;
    }

    void onShare(cocos2d::CCObject* sender)
    {
        auto level = m_fields->level;
		auto p1 = level->m_isVerifiedRaw;
		auto p2 = level->m_isVerified.value();

        if (VerifyHack::get()->getRealEnabled())
        {
		    level->m_isVerifiedRaw = true;
		    level->m_isVerified = true;
        }

		ShareLevelLayer::onShare(sender);

		level->m_isVerifiedRaw = p1;
		level->m_isVerified = p2;
    }
};