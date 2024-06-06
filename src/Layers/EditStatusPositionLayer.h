/*#pragma once

#include "../Client/Dropdown.h"
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class EditStatusPositionLayer : public FLAlertLayer
{
    public:
        static inline CCMenu* left = nullptr;
        static inline CCMenu* right = nullptr;

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        void onChange(CCObject* sender)
        {
            auto n = as<CCMenuItemSpriteExtra*>(sender);

            n->retain();

            int v = Mod::get()->getSavedValue<int>(n->getID() + std::string("_side"), 0);
            v++;

            if (v > 1)
                v = 0;

            Mod::get()->setSavedValue<int>(n->getID() + std::string("_side"), v);

            log::info("changed: {}", n->getID());

            n->removeFromParentAndCleanup(false);

            if (v == 0)
            {
                left->addChild(n);

                left->updateLayout();
                right->updateLayout();
            }
            else
            {
                right->addChild(n);

                left->updateLayout();
                right->updateLayout();
            }

            n->release();

        }

        void addStatus(std::string name, std::string id)
        {
            auto v = Mod::get()->getSavedValue<int>(id + std::string("_side"), 0);

            float max = 150;

            auto bb = ButtonSprite::create(name.c_str(), "bigFont.fnt", "GJ_button_04.png");
            bb->setScale(1.0f / (bb->getContentSize().width / max));
            bb->setScale(0.5f);

            auto btn = CCMenuItemSpriteExtra::create(bb, this, menu_selector(EditStatusPositionLayer::onChange));
            btn->setID(id);

            if (v == 0)
                left->addChild(btn);
            else
                right->addChild(btn);
        }

        bool init()
        {
            if (!FLAlertLayer::init(0))
                return false;

            this->runAction(CCFadeTo::create(1, 100));
            //this->setTouchEnabled(true);
            this->setKeypadEnabled(true);

            auto l = CCMenu::create();
            l->setContentSize(ccp(420, 270));
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

            auto title = CCLabelBMFont::create("Modify Status Text Positions", "bigFont.fnt");
            title->setPosition(l->getContentSize() / 2 + ccp(0, 122));
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChild(title);

            auto subtitle = CCLabelBMFont::create("Tap on a button to cycle its position", "chatFont.fnt");
            subtitle->setPosition(l->getContentSize() / 2 + ccp(0, 120 - 14));
            subtitle->setOpacity(100);
            subtitle->setScale(0.65f);
            l->addChild(subtitle);

            auto spr = CCSprite::create("screenshot.png"_spr);
            spr->setOpacity(175);
            spr->setPosition(l->getContentSize() / 2);
            spr->setScale(CCDirector::get()->getContentScaleFactor() * 0.175f);
            l->addChild(spr, 2);

            auto plr = SimplePlayer::create(GameManager::get()->m_playerFrame);
            
            plr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor));
            plr->setSecondColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2));
            if (GameManager::get()->m_playerGlow)
            {
                plr->setGlowOutline(GameManager::get()->colorForIdx(GameManager::get()->m_playerGlowColor));
            }

            plr->setPosition(ccp(177, 129.5f));
            plr->setRotation(404.999f);
            spr->addChild(plr);

            auto divB = CCLayerColor::create(ccc4(200, 200, 200, 35));
            divB->setContentSize(spr->getContentSize());
            divB->ignoreAnchorPointForPosition(false);
            divB->setAnchorPoint(ccp(0, 0));
            spr->addChild(divB);

            auto div = CCLayerColor::create(ccc4(200, 200, 200, 100));
            div->setPositionX(spr->getContentSize().width / 2);
            div->setContentSize(ccp(2.5f, spr->getContentSize().height));
            div->ignoreAnchorPointForPosition(false);
            div->setAnchorPoint(ccp(0.5f, 0));
            spr->addChild(div);

            auto lay = CCLayerColor::create(ccc4(0, 0, 0, 100));
            lay->setContentSize(spr->getScaledContentSize() + ccp(4, 4));
            lay->ignoreAnchorPointForPosition(false);
            lay->setPosition(l->getContentSize() / 2);
            l->addChild(lay, 1);

            auto ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(EditStatusPositionLayer::onClose));
            ok->setPosition(l->getContentSize() / 2 + ccp(0, -82 - 31.5f));
            l->addChild(ok, 5);

            left = CCMenu::create();
            left->setID("left");
            left->ignoreAnchorPointForPosition(false);
            left->setPosition(l->getContentSize() / 2 + ccp(-0.5f * spr->getScaledContentSize().width / 2, 0));
            left->setContentSize(ccp(spr->getScaledContentSize().width / 2, spr->getScaledContentSize().height));
            l->addChild(left, 5);

            right = CCMenu::create();
            right->setID("right");
            right->ignoreAnchorPointForPosition(false);
            right->setPosition(l->getContentSize() / 2 + ccp(0.5f * spr->getScaledContentSize().width / 2, 0));
            right->setContentSize(ccp(spr->getScaledContentSize().width / 2, spr->getScaledContentSize().height));
            l->addChild(right, 5);

            addStatus("Testmode", "testmode");
            addStatus("FPS Counter", "fps");
            addStatus("Noclip Deaths", "death");
            addStatus("Noclip Accuracy", "accuracy");

            left->setLayout(ColumnLayout::create()->setAutoScale(false));
            right->setLayout(left->getLayout());

            this->addChild(l);

            l->setScale(0.1f);
            l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1))); 

            handleTouchPriority(this);
    
            return true;
        }

        static EditStatusPositionLayer* create()
        {
            EditStatusPositionLayer* pRet = new EditStatusPositionLayer();
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
            auto pRet = EditStatusPositionLayer::create();

            CCScene::get()->addChild(pRet, 99999);
        }
};*/