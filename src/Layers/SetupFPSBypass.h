#pragma once

#include "../Client/Dropdown.h"
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SetupFPSBypass : public FLAlertLayer
{
    public:
        geode::InputNode* inp = nullptr;
        CCMenuItemToggler* enb = nullptr;

        void onClose(CCObject*)
        {
            if (GameManager::get()->getGameVariable("0116") != enb->isToggled())
            {
                GameManager::get()->m_customFPSTarget = std::stof(inp->getString());
                GameManager::get()->setGameVariable("0116", enb->isToggled());
                GameManager::get()->setGameVariable("0030", !enb->isToggled());

                GameManager::get()->updateCustomFPS();

                CCApplication::get()->setAnimationInterval(1.0f / GameManager::get()->m_customFPSTarget);
            }

            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        bool init()
        {
            if (!FLAlertLayer::init(0))
                return false;

            this->runAction(CCFadeTo::create(1, 100));
            //this->setTouchEnabled(true);
            this->setKeypadEnabled(true);

            auto l = CCMenu::create();
            l->setContentSize(ccp(330, 210));
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

                auto gradient = CCLayerGradient::create(ccc4(255, 255, 255, 255), ccc4(255, 255, 255, 255));
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

            auto title = CCLabelBMFont::create("Setup FPS Bypass", "bigFont.fnt");
            title->setPosition(l->getContentSize() / 2 + ccp(0, 90));
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChild(title);

            auto lb = CCLabelBMFont::create("FPS Bypass:", "bigFont.fnt");
            lb->setPosition(l->getContentSize() / 2 + ccp(-20, 30));
            lb->setScale(0.55f);
            lb->setAnchorPoint(ccp(1, 0.5f));
            l->addChild(lb);

            std::stringstream ss2;
            ss2 << GameManager::get()->m_customFPSTarget;

            /*inp = geode::InputNode::create(lb->getScaledContentSize().width, "FPS Value");
            inp->setPosition(l->getContentSize() / 2 + ccp(50, 30));
            inp->setString(ss2.str());
            inp->getInput()->setAllowedChars("098765431.");
            inp->getInput()->setMaxLabelLength(8);
            inp->getInput()->setID("IGNOREBYPASSES"_spr);
            l->addChild(inp);*/

            auto lb2 = CCLabelBMFont::create("Enabled:", "bigFont.fnt");
            lb2->setPosition(l->getContentSize() / 2 + ccp(-20, -10));
            lb2->setScale(0.55f);
            lb2->setAnchorPoint(ccp(1, 0.5f));
            l->addChild(lb2);

            enb = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.8f);
            enb->setPosition(l->getContentSize() / 2 + ccp(20, -10));
            enb->toggle(GameManager::get()->getGameVariable("0116"));
            l->addChild(enb);

            auto ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SetupFPSBypass::onClose));
            ok->setPosition(l->getContentSize() / 2 + ccp(0, -82));
            l->addChild(ok);

            this->addChild(l);

            l->setScale(0.1f);
            l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1))); 
    
            return true;
        }

        static SetupFPSBypass* create()
        {
            SetupFPSBypass* pRet = new SetupFPSBypass();
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
            auto pRet = SetupFPSBypass::create();

            CCScene::get()->addChild(pRet, 99999);
        }
};