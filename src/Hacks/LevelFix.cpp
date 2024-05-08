#include <Geode/Geode.hpp>
#include <Geode/modify/LevelTools.hpp>
#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (LevelTools)
{
    static GJGameLevel* getLevel(int p0, bool p1)
    {
        auto lvl = LevelTools::getLevel(p0, p1);
        lvl->m_levelType = GJLevelType::Local;
        lvl->m_localOrSaved = true;

        return lvl;
    }
};

/*class $modify (LevelPage)
{
    static LevelPage* create(GJGameLevel* p0)
    {
        log::info("level: {}", p0);
        
        return LevelPage::create(p0);
    }
};*/

/*Ref<LevelBrowserLayer> browser;

class $modify (LevelBrowserLayer)
{
    bool init(GJSearchObject* p0)
    {
        if (!LevelBrowserLayer::init(p0))
            return false;
        
        browser = this;

        return true;
    }
};

class $modify (LevelCell)
{

};*/