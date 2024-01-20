#include <Geode/Geode.hpp>
#include "ClientSetup.h"
using namespace geode::prelude;

class AndroidUI : public cocos2d::CCLayerColor {
public:
    static inline std::vector<CCLabelBMFont*> labels = {};
    static inline std::vector<CCMenuItemSprite*> buttons = {};
    static inline std::vector<CCMenu*> pages = {};

    static inline int selectedTab = 0;
    
    void goToPage(int p, bool transition = false)
    {
        selectedTab = p;

        if (transition)
        {

        }
        else
        {
            for (size_t i = 0; i < pages.size(); i++)
            {
                pages[i]->setVisible(i == p);
            }
            
        }
    }

    virtual void keyBackClicked()
    {
        close(nullptr);
    }

    void close(CCObject* sender)
    {
        this->removeFromParent();
    }


    CCAction* getEnterAction(CCNode* panel)
    {
        panel->setPosition(CCDirector::get()->getWinSize() / 2);
        panel->setPositionY(-1 * panel->getContentSize().height / 2);
        
        return (CCEaseElasticOut::create(CCMoveTo::create(1, CCDirector::get()->getWinSize() / 2)));
    }


    void onPressTab(CCObject* sender)
    {
        auto btn = static_cast<CCMenuItemSprite*>(sender);

        selectedTab = btn->getTag();

        for (size_t i = 0; i < labels.size(); i++)
        {
            buttons[i]->setEnabled(i != selectedTab);

            if (selectedTab == i)
            {
                labels[i]->setColor({255, 255, 255});
                labels[i]->setOpacity(255);
            }
            else
            {
                labels[i]->setColor({150, 150, 150});
                labels[i]->setOpacity(150);
            }

            labels[i]->updateLabel();
        }
        
        log::info("Changed tab to {}", selectedTab);
        goToPage(selectedTab);
    }

    virtual bool init() {
        if (!CCLayerColor::init()) {
            return false;
        }

        this->setTouchEnabled(true);
        this->setMouseEnabled(true);
        this->setKeypadEnabled(true);

        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

        this->runAction(CCFadeTo::create(0.5f, 100));

        #pragma region Back

        auto backMenu = CCMenu::create();
        backMenu->ignoreAnchorPointForPosition(false);
        backMenu->setContentSize(ccp(0, 0));
        backMenu->setPositionX(0);
        backMenu->setPositionY(CCDirector::get()->getWinSize().height);

        auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        backSpr->runAction(CCFadeIn::create(0.5f));

        auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(AndroidUI::close));
        backBtn->setPosition(ccp(20, -24));
        backBtn->setSizeMult(1.15f);

        backMenu->addChild(backBtn);
        this->addChild(backMenu);

        #pragma endregion

        auto panel = CCScale9Sprite::create("GJ_square01.png");
        panel->setContentSize(ccp(475, 280));

        auto windows = CCScale9Sprite::create("square02_small.png");
        windows->setOpacity(100);
        windows->setPosition(ccp(10, 10));
        windows->setContentSize(ccp(110, panel->getContentSize().height - 10 - 10));
        windows->setAnchorPoint(ccp(0, 0));

        auto windowsMenu = CCMenu::create();
        windowsMenu->setContentSize(ccp(windows->getContentSize().width, windows->getContentSize().height - 10));
        windowsMenu->setAnchorPoint(ccp(0, 0));
        windowsMenu->setPosition(ccp(5, 5));
        windowsMenu->ignoreAnchorPointForPosition(false);

        windowsMenu->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(3.5f));
        windows->addChild(windowsMenu);

        labels.clear();
        buttons.clear();
        pages.clear();

        for (size_t i = 0; i < Client::instance->windows.size(); i++)
        {
            auto win = Client::instance->windows[i];

            auto btn = CCScale9Sprite::create("square02_small.png");
            btn->setContentSize(ccp(100, 20) / 0.5f);
            btn->setScale(0.5f);
            btn->setOpacity(100);

            auto lbl = CCLabelBMFont::create(win->name.c_str(), "bigFont.fnt");
            lbl->setPosition(btn->getContentSize() / 2);
            lbl->limitLabelWidth(100 / 0.5f, 0.75f, 0.1f);
            lbl->setColor({200, 200, 200});

            btn->addChild(lbl);

            auto btn2 = CCScale9Sprite::create("square02_small.png");
            btn2->setContentSize(ccp(100, 20) / 0.5f);
            btn2->setScale(0.5f);
            btn2->setOpacity(100);

            auto lbl2 = CCLabelBMFont::create(win->name.c_str(), "bigFont.fnt");
            lbl2->setPosition(btn->getContentSize() / 2);

            if (selectedTab == i)
            {
                lbl2->setColor({255, 255, 255});
                lbl2->setOpacity(255);
            }
            else
            {
                lbl2->setColor({150, 150, 150});
                lbl2->setOpacity(150);
            }

            lbl2->limitLabelWidth(100 / 0.5f, 0.75f, 0.1f);

            btn2->addChild(lbl2);
            
            auto b = CCMenuItemSprite::create(btn2, btn, windowsMenu, menu_selector(AndroidUI::onPressTab));
            b->setTag(i);
            b->setEnabled(i != selectedTab);
            b->setContentSize(b->getContentSize() / 2);

            windowsMenu->addChild(b);

            labels.push_back(lbl2);
            buttons.push_back(b);
        }

        for (size_t i = 0; i < Client::instance->windows.size(); i++)
        {
            auto menu = CCMenu::create();
            menu->setAnchorPoint(ccp(1, 0));
            menu->setPosition(ccp(panel->getContentSize().width - 15, 15));
            menu->setContentSize(ccp(340, panel->getContentSize().height - 15 - 15));
            menu->ignoreAnchorPointForPosition(false);

            menu->setID(Client::instance->windows[i]->id);

            for (size_t m = 0; m < Client::instance->windows[i]->modules.size(); m++)
            {
                Client::instance->windows[i]->modules[m]->makeAndroid(menu, ccp(20, (panel->getContentSize().height - 20 - 20) - (35 * m)));
            }
            
            pages.push_back(menu);

            panel->addChild(menu);
        }

        windowsMenu->updateLayout();
        
        panel->addChild(windows);
        this->addChild(panel);

        auto versionText = CCLabelBMFont::create("Mod Developed By TheSillyDoggo", "chatFont.fnt");
        versionText->setColor({0, 0, 0});
        versionText->setOpacity(100);
        versionText->setAnchorPoint(ccp(0.5f, 0));
        versionText->setScale(0.45f);
        versionText->setPosition(ccp(64, 13));
        panel->addChild(versionText);

        goToPage(selectedTab);

        panel->runAction(getEnterAction(panel));

        return true;
    }

    // Handle touch events
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
        log::info("touch began");
        return true;
    }

    CREATE_FUNC(AndroidUI);

    static void addToScene()
    {
        auto l = AndroidUI::create();

        CCDirector::get()->getRunningScene()->addChild(l, 69420);
    }
};