#include "../../Client/Module.hpp"
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class CopyHack : public Module
{
    public:
        MODULE_SETUP(CopyHack)
        {
            setName("Copy Hack");
            setID("copy-hack");
            setCategory("Creator");
            setDescription("Allows you to copy any level from the servers");
        }
};

SUBMIT_HACK(CopyHack);

class $modify (LevelInfoLayer)
{
    bool init(GJGameLevel* level, bool challenge)
    {
        auto ps = level->m_password;

        if (CopyHack::get()->getRealEnabled())
            level->m_password = 1;

        auto pRet = LevelInfoLayer::init(level, challenge);

        level->m_password = ps;

        return pRet;
    }

    virtual void levelDownloadFinished(GJGameLevel* level)
    {
        auto ps = level->m_password;

        if (CopyHack::get()->getRealEnabled())
            level->m_password = 1;

        LevelInfoLayer::levelDownloadFinished(level);

        level->m_password = ps;
    }

    void tryCloneLevel(cocos2d::CCObject* sender)
    {
        auto ps = m_level->m_password;

        if (CopyHack::get()->getRealEnabled())
            m_level->m_password = 1;

        LevelInfoLayer::tryCloneLevel(sender);

        m_level->m_password = ps;
    }
};