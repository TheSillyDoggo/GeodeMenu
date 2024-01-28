#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Dropdown : public CCMenu
{
    static Dropdown* create(std::vector<std::string> words, CCObject* sender, SEL_MenuHandler event) {
        Dropdown *ret = new Dropdown();
        if (ret && ret->init(words, sender, event)) {
            ret->autorelease();
            return ret;
        } else {
            delete ret;
            return nullptr;
        }
    }

    virtual bool init() {
        if (!CCNode::init()) {
            return false;
        }


        
        auto sprite = Sprite::createWithSpriteFrameName("portal_17_extra_2_001.png");
        sprite->setPosition(Vec2(0, 0));
        addChild(sprite);

        // Your additional initialization code here

        return true;
    }

    /*virtual bool onTouchBegan(Touch *touch, Event *event) {
        return true;
    }*/
};

/*
class Dropdown {
    public:
        int selected = 0;
        CCMenu* menu;
        std::vector<std::string> strs;
        CCLabelBMFont* lbl;

        void onToggle(CCObject* sender) {
            auto obj = reinterpret_cast<CCMenuItemSprite*>(sender);
            bool expanded = obj->getScaleY() < 0 ? true : false;
            #ifdef GEODE_IS_WINDOWS
            obj->runAction(CCEaseBackOut::create(CCScaleTo::create(0.5f, 0.75f, (!expanded ? -0.75f : 0.75f))));
            #else
            obj->runAction(CCScaleTo::create(0.5f, 0.75f, (!expanded ? -0.75f : 0.75f)));
            #endif

            auto parent = obj->getParent();
            auto background = parent->getChildByID("background");
            auto ddmenu = parent->getChildByID("dropdown-menu");

            int h = ddmenu->getChildrenCount() + 1;
            h = expanded ? h : 1;

            ddmenu->setVisible(expanded);
            background->setContentSize({background->getContentSize().width, (25 / background->getScale()) * h});
        }

        void onSelect(CCObject* sender) {
            auto obj = reinterpret_cast<CCMenuItemSpriteExtra*>(sender);
            reinterpret_cast<CCLabelBMFont*>(obj->getParent()->getParent()->getChildByID("selected-label"))->setString((reinterpret_cast<CCLabelBMFont*>(obj->getChildren()->objectAtIndex(0)))->getString());

            auto obj2 = reinterpret_cast<CCMenuItemSprite*>(obj->getParent()->getParent()->getChildByID("flip-btn"));
            bool expanded = obj2->getScaleY() < 0 ? true : false;
            #ifdef GEODE_IS_WINDOWS
            obj2->runAction(CCEaseBackOut::create(CCScaleTo::create(0.5f, 0.75f, (!expanded ? -0.75f : 0.75f))));
            #else
            obj2->runAction(CCScaleTo::create(0.5f, 0.75f, (!expanded ? -0.75f : 0.75f)));
            #endif

            auto parent = obj2->getParent();
            auto background = parent->getChildByID("background");
            auto ddmenu = parent->getChildByID("dropdown-menu");

            int h = ddmenu->getChildrenCount() + 1;
            h = expanded ? h : 1;

            ddmenu->setVisible(expanded);
            background->setContentSize({background->getContentSize().width, (25 / background->getScale()) * h});
        }

        static Dropdown* create(std::vector<std::string> strs)
        {
            Dropdown* dd = new Dropdown();
            dd->strs = strs;

            CCSize size = {240, 25};

            CCMenu* menu = CCMenu::create();
            menu->ignoreAnchorPointForPosition(false);

            menu->setContentSize(size);

            auto background = CCScale9Sprite::create("GJ_square01.png");
            background->setScale(0.3f);
            background->setContentSize({size.width / background->getScale(), size.height / background->getScale()});
            background->setAnchorPoint({0, 1});
            background->setPosition(0, size.height);
            background->setID("background");
            background->setOpacity(100);
            background->setColor({0, 0, 0});
            menu->addChild(background);

            auto spr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
            spr->setScale(0.9f);

            auto spr2 = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
            spr2->setScale(0.9f);
            spr2->setColor({150, 150, 150});

            auto arrowBtn = CCMenuItemSprite::create(spr, spr2, menu, menu_selector(Dropdown::onToggle));
            arrowBtn->setPosition(size.width - 15, 25 / 2);
            arrowBtn->setID("flip-btn");
            arrowBtn->setScale(0.75f);
            arrowBtn->setScaleY(-0.75f);
            menu->addChild(arrowBtn);

            auto lbl = CCLabelBMFont::create(strs[0].c_str(), "bigFont.fnt");
            lbl->setScale(0.5f);
            lbl->setPosition({size.width / 2 - (arrowBtn->getScaledContentSize().width / 2) - 5, size.height / 2});
            lbl->limitLabelWidth(145, 0.6f, 0);
            lbl->setID("selected-label");
            dd->lbl = lbl;
            menu->addChild(lbl);

            auto btnMenu = CCMenu::create();
            btnMenu->setAnchorPoint({0, 1});
            btnMenu->setPosition({0, size.height});
            btnMenu->setContentSize({size.width, size.height * strs.size()});
            btnMenu->ignoreAnchorPointForPosition(false);
            btnMenu->setVisible(false);
            btnMenu->setID("dropdown-menu");

            for (size_t i = 0; i < strs.size(); i++)
            {
                auto lbl = CCLabelBMFont::create(strs[i].c_str(), "bigFont.fnt");
                lbl->limitLabelWidth(size.width - 20, 0.6f, 0);

                auto btn = CCMenuItemSpriteExtra::create(lbl, menu, menu_selector(Dropdown::onSelect));
                btn->setPosition({size.width / 2, (background->getScaledContentSize().height - (size.height * i)) + size.height / 2});

                btnMenu->addChild(btn);
            }
            

            menu->addChild(btnMenu);

            dd->menu = menu;
            return dd;
        }
};*/