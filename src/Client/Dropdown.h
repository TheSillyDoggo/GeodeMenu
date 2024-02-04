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

            bg->runAction(CCEaseInOut::create( CCContentSizeTo::create(0.35f, s), 2.0f));
        }

        bool init(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel = 0)
        {
            if (!CCMenu::init())
                return false;

            this->size = size;
            this->strs = strs;

            this->setContentSize(size);
            bg = CCScale9Sprite::create("square02b_small.png");
            bg->setAnchorPoint(ccp(0, 0));
            bg->setContentSize(size);
            bg->setColor(ccc3(0, 0, 0));
            bg->setOpacity(100);
            bg->setPositionY(size.height);
            bg->setScaleY(-1);
            this->addChild(bg);

            sprBtn = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
            sprBtn->setColor({200, 200, 200});
            sprBtn2 = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
            btn = CCMenuItemSprite::create(sprBtn, sprBtn2, this, menu_selector(Dropdown::onToggleVisible));
            btn->setPosition(size + ccp(-7.5f - sprBtn->getContentSize().width / 2, -1 * (size.height / 2)));
            this->addChild(btn);

            tex = CCLabelBMFont::create(strs[sel].c_str(), "bigFont.fnt");
            tex->setPosition(size / 2 + ccp(-1 * sprBtn->getContentSize().width, 0) + ccp(10 / 2, 0));
            tex->limitLabelWidth(size.width - 10 - (7.5f + sprBtn->getContentSize().width), 0.7f, 0.05f);
            this->addChild(tex);

            this->registerWithTouchDispatcher();
            cocos::handleTouchPriority(this);

            return true;
        }

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
};