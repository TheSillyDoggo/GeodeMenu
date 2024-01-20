#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/OptionsLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class FullOptions
{
    public:
        void OnPauseOptions(CCObject* sender)
        {
            log::info("OnPauseOptions");
        }
};

/*

class $modify (OptionsLayer)
{
    void customSetup()
    {
        OptionsLayer::customSetup();

        auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"), this, menu_selector(PauseLayer::onSettings));

        auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

        if (l)
        {
            auto m = reinterpret_cast<CCMenu*>(l->getChildren()->objectAtIndex(10));

            if (m)
            {
                btn->setPosition(ccp(-140, -65));

                m->addChild(btn);
            }
        }
    }
};

class $modify (PauseLayer)
{
    void onSettings(cocos2d::CCObject* sender)
    {
        if (Client::GetModuleEnabled("full-options"))
        {
            auto p = OptionsLayer::create();

            p->runAction(CCFadeTo::create(0.5f, 125));
            p->setVisible(true);

            auto c = reinterpret_cast<CCLayer*>(p->getChildren()->objectAtIndex(0));
            c->runAction(CCEaseInOut::create(CCMoveTo::create(0.5f, ccp(0, 0)), 2.0f));
            
            reinterpret_cast<CCMenuItemSpriteExtra*>(reinterpret_cast<CCMenu*>(c->getChildren()->objectAtIndex(10))->getChildren()->objectAtIndex(0))->setVisible(false);

            this->addChild(p, 69420);
        }
        else
            PauseLayer::onSettings(sender);
    }
};*/