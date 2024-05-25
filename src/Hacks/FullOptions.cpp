#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/OptionsLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

namespace FullOptions
{
    bool ignore = false;
};
using namespace FullOptions;

class $modify (OptionsLayerExt, OptionsLayer)
{
    void onPauseOptions(CCObject* sender)
    {
        ignore = true;

        if (auto pause = getChildOfType<PauseLayer>(CCScene::get(), 0))
        {
            pause->onSettings(sender);
        }
        
        ignore = false;
    }

    void customSetup()
    {
        OptionsLayer::customSetup();

        if (!PlayLayer::get())
            return;

        auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"), this, menu_selector(OptionsLayerExt::onPauseOptions));
        btn->setID("normal-settings-btn"_spr);

        if (auto m = getChildOfType<CCMenu>(m_mainLayer, -2))
        {
            btn->setPosition(ccp(18, -50));

            m->addChild(btn);
        }
    }
};

class $modify (PauseLayer)
{
    void onSettings(cocos2d::CCObject* sender)
    {
        if (Client::GetModuleEnabled("full-options") && !ignore)
        {
            auto layer = OptionsLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 69);
            layer->showLayer(false);
        }
        else
            PauseLayer::onSettings(sender);
    }
};