#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/CCSpriteGrayscale.hpp>
#include <Geode/modify/CCSpriteWithHue.hpp>
#include "../Client/Client.h"

class $modify(CCSpriteGrayscale)
{
    static CCSpriteGrayscale* createWithSpriteFrameName(gd::string const& spr)
    {
        auto res = CCSpriteGrayscale::createWithSpriteFrameName(spr);

        auto spr2 = CCSprite::createWithSpriteFrameName(spr.c_str());
        spr2->setPosition(res->getContentSize() / 2);

        res->addChild(spr2);
        res->setOpacity(0);
        
        return res;
    }

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        if (self.m_hooks.empty())
            return;

        auto it = self.m_hooks.begin();
        std::advance(it, 0);

        hooks.push_back(it->second.get());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("unlock-buttons");

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

#endif