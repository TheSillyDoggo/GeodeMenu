#include "../../Client/Module.hpp"
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/OptionsLayer.hpp>

using namespace geode::prelude;

class FullOptions : public Module
{
    public:
        MODULE_SETUP(FullOptions)
        {
            setID("full-options");
            setCategory("Universal");
        }

        bool ignore = false;
};

SUBMIT_HACK(FullOptions)

class $modify (OptionsLayerExt, OptionsLayer)
{
    void onPauseOptions(CCObject* sender)
    {
        FullOptions::get()->ignore = true;

        if (auto pause = CCScene::get()->getChildByType<PauseLayer>(0))
        {
            pause->onSettings(sender);
        }
        
        FullOptions::get()->ignore = false;
    }

    void customSetup()
    {
        OptionsLayer::customSetup();

        if (!PlayLayer::get())
            return;

        auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"), this, menu_selector(OptionsLayerExt::onPauseOptions));
        btn->setID("normal-settings-btn"_spr);

        if (auto m = m_mainLayer->getChildByType<CCMenu>(-2))
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
        if (FullOptions::get()->getRealEnabled() && !FullOptions::get()->ignore)
        {
            #ifdef GEODE_IS_WINDOWS
            auto event = menu_selector(MenuLayer::onOptions);

            if (event)
                (this->*event)(sender);
            #else
            auto layer = OptionsLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 69);
            layer->showLayer(false);
            #endif
        }
        else
            PauseLayer::onSettings(sender);
    }
};

// full-options