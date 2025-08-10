#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCSpriteGrayscale.hpp>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;

class UnlockButtons : public Module
{
    public:
        MODULE_SETUP(UnlockButtons)
        {
            setName("Unlock Buttons");
            setID("unlock-buttons");
            setCategory("Cosmetic");
            setDescription("Visually Unlocks the greyed out buttons in the online page");
        }
};

SUBMIT_HACK(UnlockButtons);

bool __creatorlayer__init__ = false;

class $modify(CCSpriteGrayscale)
{
    #ifdef GEODE_IS_IOS

    static CCSpriteGrayscale* createWithSpriteFrame(CCSpriteFrame* frame)
    {
        auto res = CCSpriteGrayscale::createWithSpriteFrame(frame);

        if (!__creatorlayer__init__ || !UnlockButtons::get()->getRealEnabled())
            return res;

        auto spr2 = CCSprite::createWithSpriteFrame(frame);
        spr2->setPosition(res->getContentSize() / 2);

        res->addChild(spr2);
        res->setOpacity(0);
        
        return res;
    }

    #else

    static CCSpriteGrayscale* createWithSpriteFrameName(gd::string const& spr)
    {
        auto res = CCSpriteGrayscale::createWithSpriteFrameName(spr);

        if (!__creatorlayer__init__ || !UnlockButtons::get()->getRealEnabled())
            return res;

        auto spr2 = CCSprite::createWithSpriteFrameName(spr.c_str());
        spr2->setPosition(res->getContentSize() / 2);

        res->addChild(spr2);
        res->setOpacity(0);
        
        return res;
    }

    #endif
};

class $modify (CreatorLayer)
{
    virtual bool init()
    {
        __creatorlayer__init__ = true;

        auto ret = CreatorLayer::init();

        __creatorlayer__init__ = false;

        return ret;
    }
};