#include <Geode/Geode.hpp>
#include "ClientSetup.h"
#include <Geode/modify/AchievementNotifier.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#ifdef GEODE_IS_ANDROID
#include <Geode/modify/CCScene.hpp>
#endif
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
        float v = 1.0f;

        if (SpeedhackTop::instance)
        {
            if (Client::GetModuleEnabled("speedhack-enabled"))
            {
                try
                {
                    v = std::stof(SpeedhackTop::instance->text);
                }
                catch(const std::exception& e)
                {
                    v = 1.0f;
                }

                if (v < 0.01f)
                    v = 0.01f;

                if (v > 99999)
                    v = 99999;
            }
        }

        panel->setPosition(CCDirector::get()->getWinSize() / 2);
        panel->setPositionY(-1 * panel->getContentSize().height / 2);

        return CCSpeed::create((CCEaseElasticOut::create(CCMoveTo::create(1, CCDirector::get()->getWinSize() / 2))), 1.0f / v);
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
        
        float v = 1.0f;

        if (SpeedhackTop::instance)
        {
            if (Client::GetModuleEnabled("speedhack-enabled"))
            {
                try
                {
                    v = std::stof(SpeedhackTop::instance->text);
                }
                catch(const std::exception& e)
                {
                    v = 1.0f;
                }

                if (v < 0.01f)
                    v = 0.01f;

                if (v > 99999)
                    v = 99999;
            }
        }

        backSpr->runAction(CCSpeed::create(CCFadeIn::create(0.5f), 1.0f / v));

        auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(AndroidUI::close));
        backBtn->setPosition(ccp(20, -24));
        backBtn->setSizeMult(1.15f);

        backMenu->addChild(backBtn);
        this->addChild(backMenu);

        #pragma endregion

        std::stringstream ss;
        ss << "GJ_square0";
        ss << Mod::get()->getSavedValue<int>("theme", 5);
        ss << ".png";

        auto panel = CCScale9Sprite::create(ss.str().c_str());
        panel->setContentSize(ccp(475, 280));
        panel->setID("panel");

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
            menu->setPosition(ccp(panel->getContentSize().width - 15 + 5, 10));
            menu->setContentSize(ccp(340, panel->getContentSize().height - 10 - 10));
            menu->ignoreAnchorPointForPosition(false);

            menu->setID(Client::instance->windows[i]->id);

            Client::instance->windows[i]->cocosCreate(menu);
            
            pages.push_back(menu);

            panel->addChild(menu);
        }

        windowsMenu->updateLayout();
        
        panel->addChild(windows);
        this->addChild(panel);

        std::stringstream ver;
        ver << "Using version " << Mod::get()->getVersion().getMajor() << "." << Mod::get()->getVersion().getMinor() << "." << Mod::get()->getVersion().getPatch();

        auto versionText = CCLabelBMFont::create(ver.str().c_str(), "chatFont.fnt");
        versionText->setColor({0, 0, 0});
        versionText->setOpacity(100);
        versionText->setAnchorPoint(ccp(0.5f, 0));
        versionText->setScale(0.45f);
        versionText->setPosition(ccp(64, 13 + 8));
        panel->addChild(versionText);

        auto devText = CCLabelBMFont::create("Mod Developed By TheSillyDoggo", "chatFont.fnt");
        devText->setColor({0, 0, 0});
        devText->setOpacity(100);
        devText->setAnchorPoint(ccp(0.5f, 0));
        devText->setScale(0.45f);
        devText->setPosition(ccp(64, 13));
        panel->addChild(devText);

        goToPage(selectedTab);

        panel->runAction(getEnterAction(panel));

        return true;
    }

    CREATE_FUNC(AndroidUI);

    static void addToScene()
    {
        auto l = AndroidUI::create();

        CCDirector::get()->getRunningScene()->addChild(l, 69420);
    }
};

class AndroidBall : public CCLayer
{
    public:
        static inline bool hasPos = false;
        static inline CCPoint position = ccp(32, CCDirector::get()->getWinSize().height / 2);
        static inline AndroidBall* instance = nullptr;

        bool doingThing = false;
        static inline bool dragging = false;
        CircleButtonSprite* btn;
        CCLabelBMFont* l;

        void onOpenMenu()
        {
            AndroidUI::addToScene();
        }

        virtual bool init()
        {
            if (!CCLayer::init())
                return false;

            this->setID("android-ball");

            this->setTouchEnabled(true);
            this->setMouseEnabled(true);

            CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
            //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 69420);

            auto menu = CCMenu::create();
            menu->setPosition(ccp(0, 0));
            menu->setContentSize(ccp(0, 0));

            l = CCLabelBMFont::create(">_", "bigFont.fnt");
            l->setAnchorPoint(ccp(0.5f, 0.35f));

            btn = CircleButtonSprite::create(l);
            btn->setPosition(position);
            menu->addChild(btn);

            this->addChild(menu);

            this->setZOrder(69420 - 1);

            this->scheduleUpdate();

            UpdateVisible(true);

            return true;
        }

        virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
            auto space = btn->convertTouchToNodeSpace(touch);

            doingThing = false;
            dragging = false;

            if (space.x > 0 && space.y > 0)
            {
                if (space.x < btn->getContentSize().width && space.y < btn->getContentSize().height)
                {
                    btn->runAction(CCEaseInOut::create(CCScaleTo::create(0.1f, 0.8f), 2));
                    doingThing = true;
                }
            }

            return doingThing;
        }

        virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
            if (doingThing)
            {
                if (!dragging)
                    onOpenMenu();

                btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1)));

                doingThing = false;
            }
        }

        virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
            if (doingThing && (btn->numberOfRunningActions() == 0))
            {
                if (btn->getPosition().getDistance(touch->getLocation()) > 7.5f)
                {
                    dragging = true;

                    //log::info("dragging");
                }

                if (dragging)
                {
                    position = touch->getLocation();

                    btn->setPosition(position);
                }
            }
        }

        virtual void update(float dt)
        {
            ColourUtility::pastel++;

            l->setColor(ColourUtility::getPastelColour(ColourUtility::pastel));
            instance = this;

            UpdateVisible(false);
        }

        void UpdateVisible(bool i)
        {
            if (btn->numberOfRunningActions() != 0)
                return;

            if (i)
            {
                if (PlayLayer::get())
                {
                    btn->setOpacity(50);
                    l->setOpacity(50);
                }
            }

            int op = 255;

            if (PlayLayer::get())
            {
                if (CCDirector::get()->getRunningScene()->getChildByID("pause-layer"))
                {
                    op = 255;
                }
                else
                {
                    op = 50;
                }
            }
            else if (GameManager::sharedState()->m_levelEditorLayer)
            {
                if (CCDirector::get()->getRunningScene()->getChildByID("editor-pause-layer"))
                {
                    op = 255;
                }
                else
                {
                    op = 50;
                }
            }

            if (op != btn->getOpacity())
            {
                btn->runAction(CCFadeTo::create(0.5f, op));
                l->runAction(CCFadeTo::create(0.5f, op));
            }
        }

        CREATE_FUNC(AndroidBall);
};

/*#ifdef GEODE_IS_ANDROID

class $modify (CCScene)
{
    int getHighestChildZ()
    {
        AndroidBall::instance->removeFromParentAndCleanup(false);

        int v = CCScene::getHighestChildZ();

        this->addChild(AndroidBall::instance);

        return v;
    }
};

#endif*/

class $modify (PlayLayer)
{
    void pauseGame(bool p0)
    {
        PlayLayer::pauseGame(p0);

        log::info("paused");

        if (AndroidBall::instance)
        {
            log::info("exists");

            AndroidBall::instance->removeFromParent();
        }

        auto andBall = AndroidBall::create();
        CCDirector::get()->getRunningScene()->addChild(andBall);
    }
};

class $modify (PauseLayer)
{
    static PauseLayer* create(bool p0)
    {
        auto v = PauseLayer::create(p0);

        v->setID("pause-layer");

        return v;
    }
};

//todo: fix android
/*
class $modify (MenuLaunchFix, MenuLayer)
{
    void fix(float dt)
    {
        log::info("fix unclickable");

        if (AndroidBall::instance)
            AndroidBall::instance->removeFromParent();

        CCDirector::get()->getRunningScene()->addChild(AndroidBall::create());
    }

    virtual bool init()
    {
        if (!MenuLayer::init())
            return false;

        this->scheduleOnce(schedule_selector(MenuLaunchFix::fix), 0.1f);

        return true;
    }
};
*/
class $modify (AchievementNotifier)
{
    void willSwitchToScene(CCScene* p0)
    {
        log::info("will switch to scene");

        AchievementNotifier::willSwitchToScene(p0);

        auto andBall = AndroidBall::create();
        p0->addChild(andBall);

        return;

        if (!p0->getChildByID("android-ball"))
            p0->addChild(AndroidBall::create());
        else
        {
            AndroidBall::instance = static_cast<AndroidBall*>(p0->getChildByID("android-ball"));
            AndroidBall::instance->btn->setPosition(AndroidBall::instance->position);
        }
    }
};