#include <Geode/Geode.hpp>
#include <Geode/modify/CCSpriteGrayscale.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../Client/Client.h"

bool __creatorlayer__init__ = false;

class $modify(CCSpriteGrayscale)
{
    #ifdef GEODE_IS_IOS

    static CCSpriteGrayscale* createWithSpriteFrame(CCSpriteFrame* frame)
    {
        auto res = CCSpriteGrayscale::createWithSpriteFrame(frame);

        if (!__creatorlayer__init__)
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

        if (!__creatorlayer__init__)
            return res;

        auto spr2 = CCSprite::createWithSpriteFrameName(spr.c_str());
        spr2->setPosition(res->getContentSize() / 2);

        res->addChild(spr2);
        res->setOpacity(0);
        
        return res;
    }

    #endif

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        if (self.m_hooks.empty())
            return;

        auto it = self.m_hooks.begin();
        std::advance(it, 0);

        hooks.push_back(it->second.get());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Module::get("unlock-buttons");

            for (auto hook : hooks)
            {
                if (hook)
                {
                    modu->addHook(hook);
                }
            }
        });
    }
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