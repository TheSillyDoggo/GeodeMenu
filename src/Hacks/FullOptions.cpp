#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/OptionsLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

bool ig = false;

class $modify (OptionsLayerExt, OptionsLayer)
{
    void onPauseOptions(CCObject* sender)
    {
        ig = true;

        if (auto pause = getChildOfType<PauseLayer>(CCScene::get(), 0))
        {
            pause->onSettings(sender);
        }
        
        ig = false;
    }

    void customSetup()
    {
        OptionsLayer::customSetup();

        if (!PlayLayer::get())
            return;

        auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"), this, menu_selector(OptionsLayerExt::onPauseOptions));

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
        if (Client::GetModuleEnabled("full-options") && !ig)
        {
            auto layer = OptionsLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 69);
            layer->showLayer(false);
        }
        else
            PauseLayer::onSettings(sender);
    }
};