#include "../../Client/Module.hpp"
#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/LevelAreaInnerLayer.hpp>

using namespace geode::prelude;

class MainLevelBypass : public Module
{
    public:
        MODULE_SETUP(MainLevelBypass)
        {
            setID("main-level-bypass");
            setCategory("Universal");
        }
};

SUBMIT_HACK(MainLevelBypass)

class $modify (LevelPage)
{
    void onPlay(cocos2d::CCObject* sender)
    {
        auto v = m_level->m_requiredCoins;

        if (MainLevelBypass::get()->getRealEnabled())
            m_level->m_requiredCoins = 0;

        LevelPage::onPlay(sender);

        m_level->m_requiredCoins = v;
    }
};

class $modify (LevelAreaInnerLayer)
{
    bool init(bool p0)
    {
        if (!LevelAreaInnerLayer::init(p0))
            return false;

        if (!MainLevelBypass::get()->getRealEnabled())
            return true;

        auto x = this->getChildByType<CCNode>(1);
        auto menu = x->getChildByType<CCMenu>(0);

        if (menu)
        {
            CCArrayExt<CCNode*> objs = menu->getChildren();

            for (auto child : objs)
            {
                if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(child))
                    btn->setEnabled(true);
            }
        }

        return true;
    }
};