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

            std::stringstream ss;
            ss << "GJ_square0";
            ss << Mod::get()->getSavedValue<int>("theme", 5);
            ss << ".png";

            auto panel = CCScale9Sprite::create(ss.str().c_str());
            panel->setContentSize(l->getContentSize());
            panel->setAnchorPoint(ccp(0, 0));
            panel->setID("panel");
            l->addChild(panel);

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