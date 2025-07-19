/*#include "../../Client/Module.hpp"
#include <Geode/modify/ShareLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class NoCopyMark : public Module
{
    public:
        MODULE_SETUP(NoCopyMark)
        {
            setName("No Copy Mark");
            setID("noc-hack");
            setCategory("Creator");
            setDescription("Hides the (c) mark from your levels on publish & copy");
        }
};

SUBMIT_HACK(NoCopyMark);

class $modify (ShareLevelLayer)
{
    struct Fields
    {
        // i have the offset to put this as an actual member but im not doing anything for the geode developers cuz they unverified me for no reason
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
        auto og = m_fields->level->m_originalLevel;

        if (NoCopyMark::get()->getRealEnabled())
            m_fields->level->m_originalLevel = 0;

        ShareLevelLayer::onShare(sender);

        m_fields->level->m_originalLevel = og;
    }
};

class $modify (LevelInfoLayer)
{
    virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1)
    {
        if (p0->getTag() == 6 && p1)
        {
            auto og = m_level->m_originalLevel;

            if (NoCopyMark::get()->getRealEnabled())
                m_level->m_originalLevel = 0;

            LevelInfoLayer::FLAlert_Clicked(p0, p1);

            m_level->m_originalLevel = og;
            return;
        }

        LevelInfoLayer::FLAlert_Clicked(p0, p1);
    }
};*/