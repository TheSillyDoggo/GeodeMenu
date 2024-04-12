#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SetupFadeSetting : public FLAlertLayer
{
    public:
        int mode = -1;
        CCScale9Sprite* left = nullptr;
        CCScale9Sprite* right = nullptr;

        bool leftA = false;

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        void update(float dt)
        {
            std::stringstream fadeIn;
            fadeIn << "fadeColour1";
            fadeIn << mode;

            std::stringstream fadeOut;
            fadeOut << "fadeColour2";
            fadeOut << mode;

            ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn.str(), {0, 0, 0});
            ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut.str(), {255, 255, 255});

            left->setColor(in);
            right->setColor(out);
        }

        void onColour(CCObject* sender)
        {
            leftA = (static_cast<CCNode*>(sender)->getTag() == 0);

            std::stringstream fadeIn;
            fadeIn << "fadeColour1";
            fadeIn << mode;

            std::stringstream fadeOut;
            fadeOut << "fadeColour2";
            fadeOut << mode;

            ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn.str(), {0, 0, 0});
            ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut.str(), {255, 255, 255});

            auto pop = ColorPickPopup::create((leftA ? in : out));
            pop->show();
            //auto pop = geode::ColorPickPopup::create({255, 0, 0, 255}, false);
            //CCScene::get()->addChild(pop, this->getZOrder() + 1);
        }

        bool init(int mode)
        {
            if (!FLAlertLayer::init(0))
                return false;

            this->mode = mode;

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

            auto title = CCLabelBMFont::create("Setup Icon Fade\nCustomising speed and colours is not yet supported", "bigFont.fnt");
            title->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            title->updateLabel();
            title->setPosition(l->getContentSize() / 2 + ccp(0, 90));
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChild(title);

            auto ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SetupFadeSetting::onClose));
            ok->setPosition(l->getContentSize() / 2 + ccp(0, -82));
            l->addChild(ok);


            left = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            left->setContentSize(ccp(90, 35));
            auto leftBtn = CCMenuItemSpriteExtra::create(left, this, menu_selector(SetupFadeSetting::onColour));
            leftBtn->setEnabled(false);
            leftBtn->setTag(0);
            leftBtn->setPosition(l->getContentSize() / 2 + ccp(-80, -15));
            l->addChild(leftBtn);

            right = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            right->setContentSize(ccp(90, 35));
            auto rightBtn = CCMenuItemSpriteExtra::create(right, this, menu_selector(SetupFadeSetting::onColour));
            rightBtn->setEnabled(false);
            rightBtn->setTag(1);
            rightBtn->setPosition(l->getContentSize() / 2 + ccp(80, -15));
            l->addChild(rightBtn);

            this->addChild(l);

            l->setScale(0.1f);
            l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1)));       
            this->scheduleUpdate();     
    
            return true;
        }

        static SetupFadeSetting* create(int mode)
        {
            SetupFadeSetting* pRet = new SetupFadeSetting();
            if (pRet && pRet->init(mode)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }
};