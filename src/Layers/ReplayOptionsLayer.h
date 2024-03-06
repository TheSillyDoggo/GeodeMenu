#pragma once

#include "../Client/Dropdown.h"
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class ReplayOptionsLayer : public FLAlertLayer
{
    public:
        CCMenu* l = nullptr;
        std::vector<CCMenuItemToggler*> toggles = {};

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        void onToggle(CCObject*)
        {
            Mod::get()->setSavedValue<bool>("click-fixes", toggles[0]->isToggled());
            Mod::get()->setSavedValue<bool>("frame-fixes", toggles[1]->isToggled());

            if (!Mod::get()->saveData().isOk())
                log::info("womp womp");
        }

        void addToggle(std::string name, int x, int y, bool enabled)
        {
            auto pos = ccp(100 * x, -55 - (19 * y) - (19 / 2));

            auto toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(ReplayOptionsLayer::onToggle), 0.45f);
            toggler->toggle(enabled);
            auto lbl = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
            lbl->limitLabelWidth(90 - 30, 0.65f, 0);
            lbl->setAnchorPoint(ccp(0, 0.5f));

            l->addChildAtPosition(toggler, Anchor::Top, pos + ccp(-30 - 5, 0));
            l->addChildAtPosition(lbl, Anchor::Top, pos + ccp(-30 + 5, 0));

            toggles.push_back(toggler);
        }

        bool init()
        {
            if (!FLAlertLayer::init(0))
                return false;

            this->runAction(CCFadeTo::create(1, 100));
            //this->setTouchEnabled(true);
            this->setKeypadEnabled(true);

            l = CCMenu::create();
            l->setContentSize(ccp(330, 235));
            l->setPosition(CCDirector::get()->getWinSize() / 2);
            l->ignoreAnchorPointForPosition(false);
            l->setTouchPriority(-514);

            int theme = Mod::get()->getSavedValue<int>("theme", 5);

            std::stringstream ss;
            ss << "GJ_square0";
            ss << (theme == -1 ? 6 : theme);
            ss << ".png";

            auto panel = CCScale9Sprite::create(ss.str().c_str());
            panel->setContentSize(l->getContentSize());
            panel->setAnchorPoint(ccp(0, 0));
            panel->setID("panel");
            l->addChild(panel);

            as<CCNode*>(panel->getChildren()->objectAtIndex(0))->setZOrder(-2);

            if (Loader::get()->getLoadedMod("TheSillyDoggo.GradientPages"))
            {
                auto size = panel->getContentSize();

                auto gradient = CCLayerGradient::create();
                gradient->setContentSize(size);
                gradient->setZOrder(-1);
                gradient->setID("gradient"_spr);

                if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
                {
                    gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
                    gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
                }
                else
                {
                    gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
                    gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
                }

                gradient->setPosition(CCDirector::get()->getWinSize() / 2);
                gradient->ignoreAnchorPointForPosition(false);

                if (Mod::get()->getSettingValue<bool>("reverse-order"))
                gradient->setScaleY(-1);

                auto darken = CCScale9Sprite::createWithSpriteFrameName((std::string("TheSillyDoggo.GradientPages/") + std::string("square-fill.png")).c_str());
                darken->setID("darken"_spr);
                darken->setContentSize(size - ccp(15, 15));
                darken->setZOrder(0);
                darken->setPosition(size / 2);

                auto outline = CCScale9Sprite::createWithSpriteFrameName((std::string("TheSillyDoggo.GradientPages/") + std::string("square-outline.png")).c_str());
                outline->setPosition(size / 2);
                outline->setContentSize(size);
                outline->setZOrder(1);
                outline->setID("outline"_spr);
                
                gradient->addChild(darken);
                gradient->addChild(outline);

                panel->addChild(gradient);

                gradient->setAnchorPoint(ccp(0, 0));
                gradient->setPosition(ccp(0, 0));

                gradient->setVisible(theme == -1);
            }

            auto title = CCLabelBMFont::create("Replay Options", "bigFont.fnt");
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChildAtPosition(title, Anchor::Top, ccp(0, -12));

            auto clickBG = CCScale9Sprite::create("square02_001.png");
            clickBG->setOpacity(100);
            clickBG->setContentSize(ccp(90, 130) / 0.5f);
            clickBG->setScale(0.5f);
            l->addChildAtPosition(clickBG, Anchor::Center, ccp(-100, 0));

            auto frameBG = CCScale9Sprite::create("square02_001.png");
            frameBG->setOpacity(100);
            frameBG->setContentSize(ccp(90, 130) / 0.5f);
            frameBG->setScale(0.5f);
            l->addChildAtPosition(frameBG, Anchor::Center, ccp(0, 0));

            auto miscBG = CCScale9Sprite::create("square02_001.png");
            miscBG->setOpacity(100);
            miscBG->setContentSize(ccp(90, 130) / 0.5f);
            miscBG->setScale(0.5f);
            l->addChildAtPosition(miscBG, Anchor::Center, ccp(100, 0));

            addToggle("Click Fixes", -1, 0, Mod::get()->getSavedValue<bool>("click-fixes", true));
            addToggle("Frame Fixes", -1, 1, Mod::get()->getSavedValue<bool>("frame-fixes", true));
            //addToggle("ASDF", 0, 1, false);
            //addToggle("ASDF", 0, 2, false);
            //addToggle("ASDF", 0, 3, false);
            //addToggle("ASDF", 0, 4, false);
            //addToggle("ASDF", 0, 5, false);
            //addToggle("ASDF", 1, 0, false);

            auto ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(ReplayOptionsLayer::onClose));
            l->addChildAtPosition(ok, Anchor::Bottom, ccp(0, 25));

            this->addChild(l);

            //l->setScale(0.1f);
            //l->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.65f, 1)));
    
            return true;
        }

        static ReplayOptionsLayer* create()
        {
            ReplayOptionsLayer* pRet = new ReplayOptionsLayer();
            if (pRet && pRet->init()) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }

        static void addToScene()
        {
            auto pRet = ReplayOptionsLayer::create();

            CCScene::get()->addChild(pRet, 99999);
        }
};