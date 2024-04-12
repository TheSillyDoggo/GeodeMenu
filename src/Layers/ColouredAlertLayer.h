#pragma once

#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class ColouredAlertLayer : public FLAlertLayer
{
    public:
        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        bool init(std::string title, std::string str)
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

            auto content = CCLabelBMFont::create(str.c_str(), "chatFont.fnt");
            content->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            l->addChild(content);

            panel->setContentSize(content->getContentSize() + ccp(0, 0));

            auto titleL = CCLabelBMFont::create(title.c_str(), "bigFont.fnt");
            titleL->setPosition(l->getContentSize() / 2 + ccp(0, 90));
            titleL->setOpacity(100);
            titleL->setScale(0.5f);
            l->addChild(titleL);

            auto ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(ColouredAlertLayer::onClose));
            ok->setPosition(ccp(l->getContentSize().width / 2, 5));
            l->addChild(ok);

            this->addChild(l);

            l->setScale(0.1f);
            l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1))); 
    
            return true;
        }

        static ColouredAlertLayer* create(std::string title, std::string str)
        {
            ColouredAlertLayer* pRet = new ColouredAlertLayer();
            if (pRet && pRet->init(title, str)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }

        static void addToScene(std::string title, std::string str)
        {
            auto pRet = ColouredAlertLayer::create(title, str);

            CCScene::get()->addChild(pRet, 99999);
        }
};