#pragma once

#include "../Client/Dropdown.h"
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SetupTransCustom : public FLAlertLayer
{
    public:
        std::vector<std::string> transNames = {"Fade", "Cross Fade", "Fade Bottom Left", "Fade Top Right", "Fade Up", "Fade Down", "Flip Angular", "Flip X", "Flip Y", "Zoom Flip Angular", "Zoom Flip X", "Zoom Flip Y", "Jump Zoom", "Move In Top", "Move In Bottom", "Move In Left", "Move In Right", "Rotate Zoom", "Shrink Grow", "Slide In Top", "Slide In Bottom", "Slide In Left", "Slide In Right", "Split Rows", "Split Columns", "Tiles"};

        CCMenuItemSpriteExtra* left;
        CCMenuItemSpriteExtra* right;
        CCLabelBMFont* label;

        int selMode;

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        void onLeft(CCObject*)
        {
            selMode--;

            left->setVisible(selMode != 0);
            right->setVisible(selMode != transNames.size() - 1);
            label->setString(transNames[selMode].c_str());
            label->limitLabelWidth(235, 1, 0.1f);

            Mod::get()->setSavedValue<int>("transition", selMode);
        }

        void onRight(CCObject*)
        {
            selMode++;

            left->setVisible(selMode != 0);
            right->setVisible(selMode != transNames.size() - 1);
            label->setString(transNames[selMode].c_str());
            label->limitLabelWidth(235, 1, 0.1f);

            Mod::get()->setSavedValue<int>("transition", selMode);
        }

        bool init()
        {
            if (!FLAlertLayer::init(0))
                return false;

            selMode = Mod::get()->getSavedValue<int>("transition", 0);

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

            auto title = CCLabelBMFont::create("Transition Customizer", "bigFont.fnt");
            title->setPosition(l->getContentSize() / 2 + ccp(0, 90));
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChild(title);

            auto ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SetupTransCustom::onClose));
            ok->setPosition(l->getContentSize() / 2 + ccp(0, -82));
            l->addChild(ok);

            auto leftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            left = CCMenuItemSpriteExtra::create(leftSpr, this, menu_selector(SetupTransCustom::onLeft));
            left->setPosition(l->getContentSize() / 2 + ccp(-140, 0));
            left->setVisible(selMode != 0);
            l->addChild(left);

            auto rightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
            rightSpr->setFlipX(true);
            right = CCMenuItemSpriteExtra::create(rightSpr, this, menu_selector(SetupTransCustom::onRight));
            right->setPosition(l->getContentSize() / 2 + ccp(140, 0));
            right->setVisible(selMode != transNames.size() - 1);
            l->addChild(right);

            label = CCLabelBMFont::create(transNames[selMode].c_str(), "bigFont.fnt");
            label->setPosition(l->getContentSize() / 2);
            label->limitLabelWidth(235, 1, 0.1f);
            l->addChild(label);

            //auto dd = Dropdown::create({130, 30}, , nullptr);
            //dd->setTouchPriority(-550);
            //l->addChild(dd);

            this->addChild(l);

            l->setScale(0.1f);
            l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1))); 
    
            return true;
        }

        static SetupTransCustom* create()
        {
            SetupTransCustom* pRet = new SetupTransCustom();
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
            auto pRet = SetupTransCustom::create();

            CCScene::get()->addChild(pRet, 99999);
        }
};