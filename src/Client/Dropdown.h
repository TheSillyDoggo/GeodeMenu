#pragma once

#include "Client.h"

#include <Geode/Geode.hpp>
#include "CCContentSizeTo.h"
#include "ClippingNode.h"

using namespace geode::prelude;

class Dropdown : public /*CCMenu*/ClippingNode {
    private:
        CCLabelBMFont* tex;
        CCSize size;
        CCScale9Sprite* bg;
        CCSprite* sprBtn;
        CCSprite* sprBtn2;
        CCMenuItemSprite* btn;

        int selected = 0;
        bool open;
        std::vector<std::string> strs;
        std::vector<CCMenuItemSprite*> btns;

        cocos2d::SEL_MenuHandler event;

    public:
        void onToggleVisible(CCObject*)
        {
            open = !open;

            btn->stopAllActions();

            if (open)
            {
                btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, -1)));

                setVis(true);
            }
            else
            {
                btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, 1)));

                setVis(false);
            }
        }

        void setVis(bool n)
        {
            CCPoint s = ccp(size.width, size.height * (1 + (open ? strs.size() : 0)));
            bg->stopAllActions();
            bg->runAction(CCEaseInOut::create( CCContentSizeTo::create(0.35f, s / 0.5f), 2.0f));

            if (n)
            {
                for (size_t i = 0; i < btns.size(); i++)
                {
                    btns[i]->stopAllActions();
                    btns[i]->setEnabled(n);

                    //btns[i]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f * i), CCEaseInOut::create(CCScaleTo::create(0.5f, 1), 2)));
                }
            }
            else
            {
                std::reverse(btns.begin(), btns.end());
                
                for (size_t i = 0; i < btns.size(); i++)
                {
                    btns[i]->stopAllActions();
                    btns[i]->setEnabled(n);

                    //btns[i]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f * i), CCEaseInOut::create(CCScaleTo::create(0.35f, 0), 2)));
                }

                std::reverse(btns.begin(), btns.end());
            }
        }

        void onPress(CCObject* sender)
        {
            open = false;
            setSelected(as<CCNode*>(sender)->getTag());

            btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, 1)));
            setVis(false);

            if (event)
                (this->*event)(sender);
        }

        bool init(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel = 0)
        {
            if (!CCMenu::init())
                return false;

            this->size = size;
            this->strs = strs;
            this->event = callback;

            this->setContentSize(size);
            bg = CCScale9Sprite::create("square02b_small.png");
            bg->setAnchorPoint(ccp(0, 0));
            bg->setContentSize(size / 0.5f);
            bg->setScale(0.5f);
            bg->setColor(ccc3(0, 0, 0));
            bg->setOpacity(200);
            bg->setPositionY(size.height);
            bg->setScaleY(-1 * 0.5f);
            this->addChild(bg);

            sprBtn = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
            sprBtn->setColor({200, 200, 200});
            sprBtn2 = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
            btn = CCMenuItemSprite::create(sprBtn, sprBtn2, this, menu_selector(Dropdown::onToggleVisible));
            btn->setPosition(size + ccp(-7.5f - sprBtn->getContentSize().width / 2, -1 * (size.height / 2)));
            this->addChild(btn);

            tex = CCLabelBMFont::create(strs[sel].c_str(), "bigFont.fnt");
            tex->setPosition(size / 2 + ccp(-1 * sprBtn->getContentSize().width, 0) + ccp(10 / 2, 0));
            tex->limitLabelWidth(size.width - 10 - (7.5f + sprBtn->getContentSize().width), 0.7f, 0.05f);
            this->addChild(tex);
            
            for (size_t s = 0; s < strs.size(); s++)
            {
                auto lbl = CCLabelBMFont::create(strs[s].c_str(), "bigFont.fnt");
                lbl->limitLabelWidth(size.width - 10 - 10, 0.7f, 0.01f);
                lbl->setColor(ccc3(200, 200, 200));

                auto btn = CCMenuItemSpriteExtra::create(lbl, this, menu_selector(Dropdown::onPress));
                btn->setPosition(ccp(size.width / 2, (size.height * (s + 1) * -1) + size.height / 2 ));
                btn->setScale(1);
                btn->setTag(s);
                btn->setEnabled(false);
                this->addChild(btn);

                btns.push_back(btn);
            }            

            this->registerWithTouchDispatcher();
            this->scheduleUpdate();
            cocos::handleTouchPriority(this);

            return true;
        }

        int getSelectedIndex() { return selected; }

        void setSelected(int sh)
        {
            selected = sh;

            auto ss = (strs[sh]);
            tex->setString(ss.c_str());
            tex->limitLabelWidth(size.width - 10 - (7.5f + sprBtn->getContentSize().width), 0.7f, 0.05f);
        }

        void update(float dt)
        {
            this->setClipRect(CCRectMake(this->getParent()->convertToWorldSpace(this->getPosition()).x - 50, (this->getParent()->convertToWorldSpace(this->getPosition()).y - (bg->getContentSize().height / 2) + size.height) * getScaleY(), (size.width + 50) * getScaleX() + 50, (size.height * (strs.size() + 2)) * getScaleY()));
        }

        static Dropdown* create(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel = 0) {
            Dropdown* ret = new Dropdown();
            if (ret && ret->init(size, strs, callback, sel)) {
                ret->autorelease();
                return ret;
            } else {
                delete ret;
                ret = nullptr;
                return nullptr;
            }
        }        
};

/*
#pragma once

#include <Geode/Geode.hpp>
#include "CCContentSizeTo.h"

using namespace geode::prelude;

class Dropdown : public CCMenu {
    private:
        CCLabelBMFont* tex;
        CCSize size;
        CCScale9Sprite* bg;
        CCSprite* sprBtn;
        CCSprite* sprBtn2;
        CCMenuItemSprite* btn;

        int selected = 0;
        bool open;
        std::vector<std::string> strs;
        std::vector<CCMenuItemSprite*> btns;

        cocos2d::SEL_MenuHandler event;

    public:
        void onToggleVisible(CCObject*)
        {
            open = !open;

            btn->stopAllActions();

            if (open)
            {
                btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, -1)));

                setVis(true);
            }
            else
            {
                btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, 1)));

                setVis(false);
            }
        }

        void setVis(bool n)
        {
            CCPoint s = ccp(size.width, size.height * (1 + (open ? (std::min<float>(6, strs.size())) : 0)));
            bg->stopAllActions();
            bg->runAction(CCEaseInOut::create( CCContentSizeTo::create(0.35f, s), 2.0f));

            if (n)
            {
                for (size_t i = 0; i < btns.size(); i++)
                {
                    btns[i]->stopAllActions();

                    btns[i]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.02f * i), CCEaseInOut::create(CCScaleTo::create(0.5f, 1), 2)));
                }
            }
            else
            {
                std::reverse(btns.begin(), btns.end());
                
                for (size_t i = 0; i < btns.size(); i++)
                {
                    btns[i]->stopAllActions();

                    btns[i]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.02f * i), CCEaseInOut::create(CCScaleTo::create(0.35f, 0), 2)));
                }

                std::reverse(btns.begin(), btns.end());
            }
        }

        void onPress(CCObject* sender)
        {
            open = false;
            setSelected(as<CCNode*>(sender)->getTag());

            btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, 1)));
            setVis(false);

            if (event)
                (this->*event)(sender);
        }

        bool init(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel = 0)
        {
            if (!CCMenu::init())
                return false;

            this->size = size;
            this->strs = strs;
            this->event = callback;

            this->setContentSize(size);
            bg = CCScale9Sprite::create("square02b_small.png");
            bg->setAnchorPoint(ccp(0, 0));
            bg->setContentSize(size);
            bg->setColor(ccc3(0, 0, 0));
            bg->setOpacity(100);
            bg->setPositionY(size.height);
            bg->setScaleY(-1);
            this->addChild(bg);

            sprBtn = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
            sprBtn->setColor({200, 200, 200});
            sprBtn2 = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
            btn = CCMenuItemSprite::create(sprBtn, sprBtn2, this, menu_selector(Dropdown::onToggleVisible));
            btn->setPosition(size + ccp(-7.5f - sprBtn->getContentSize().width / 2, -1 * (size.height / 2)));
            this->addChild(btn);

            tex = CCLabelBMFont::create(strs[sel].c_str(), "bigFont.fnt");
            tex->setPosition(size / 2 + ccp(-1 * sprBtn->getContentSize().width, 0) + ccp(10 / 2, 0));
            tex->limitLabelWidth(size.width - 10 - (7.5f + sprBtn->getContentSize().width), 0.7f, 0.05f);
            this->addChild(tex);

            auto scl = geode::ScrollLayer::create({ size.width, size.height * std::min<float>(6, strs.size()) });
            scl->setPositionY(-1 * scl->getContentSize().height);
            scl->m_contentLayer->setContentSize(ccp(scl->getContentSize().width, size.height * strs.size()));
            scl->moveToTop();

            auto m = CCMenu::create();

            for (size_t s = 0; s < strs.size(); s++)
            {
                auto lbl = CCLabelBMFont::create(strs[s].c_str(), "bigFont.fnt");
                lbl->limitLabelWidth(size.width - 10 - 10, 0.7f, 0.01f);

                auto btn = CCMenuItemSpriteExtra::create(lbl, this, menu_selector(Dropdown::onPress));
                btn->setPosition(ccp(size.width / 2, scl->m_contentLayer->getContentSize().height + ((size.height * (s + 1) * -1) + size.height / 2 )));
                btn->setScale(0);
                btn->setTag(s);
                btn->setSizeMult(1.1f);
                m->addChild(btn);

                btns.push_back(btn);
            }

            scl->m_contentLayer->addChild(m);
            scl->setTouchPriority(-555);

            this->addChild(scl);

            this->registerWithTouchDispatcher();
            //cocos::handleTouchPriority(this); // maybe this fixes it

            return true;
        }

        int getSelectedIndex() { return selected; }

        void setSelected(int sh)
        {
            selected = sh;

            auto ss = (strs[sh]);
            tex->setString(ss.c_str());
            tex->limitLabelWidth(size.width - 10 - (7.5f + sprBtn->getContentSize().width), 0.7f, 0.05f);
        }

        void update(float dt)
        {
            
        }

        static Dropdown* create(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel = 0) {
            Dropdown* ret = new Dropdown();
            if (ret && ret->init(size, strs, callback, sel)) {
                ret->autorelease();
                return ret;
            } else {
                delete ret;
                ret = nullptr;
                return nullptr;
            }
        }        
};*/