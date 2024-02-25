#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "MacroCell.h"
#include "../Client/Client.h"

using namespace geode::prelude;

class LoadMacroPopup : public FLAlertLayer, TextInputDelegate
{
    public:
        CCLabelBMFont* error;
        ScrollLayer* scroll;

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        void onSearch(CCObject*)
        {

        }

        void onRefresh(CCObject*)
        {
            this->removeFromParent();

            auto a = LoadMacroPopup::addToScene();
            a->stopAllActions();
            a->setOpacity(100);

            CCScene::get()->addChild(TextAlertPopup::create("Refreshed Macros", 0.5f, 0.6f, 150, ""), 9999999);
        }

        void onFolder(CCObject*)
        {
            geode::createQuickPopup(
                "Open Macro Directory?",
                "Are you sure you want to <cg>open</c> the <cl>macro</c> directory",
                "No", "Yes",
                [](auto, bool btn2) {
                    if (btn2) {
                        utils::file::openFolder(Mod::get()->getConfigDir() / "macros");
                    }
                }
            );
        }

        void onMacroInfo(CCObject*)
        {
            auto al = FLAlertLayer::create("Load Macro", "This is the load macro help, <cl>incredible</c>", "OK");
            al->setTouchPriority(-515);
            al->show();
        }

        bool init()
        {
            if (!FLAlertLayer::init(0))
                return false;

            this->runAction(CCFadeTo::create(1, 100));
            //this->setTouchEnabled(true);
            this->setKeypadEnabled(true);

            auto l = CCMenu::create();
            l->setContentSize(ccp(400, 260));
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

            auto title = CCLabelBMFont::create("Load Macro", "bigFont.fnt");
            title->setPosition(l->getContentSize() / 2 + ccp(0, (l->getContentSize().height / 2) - 15));
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChild(title);

            auto cancel = CCMenuItemSpriteExtra::create(ButtonSprite::create("Cancel"), this, menu_selector(LoadMacroPopup::onClose));
            cancel->setPosition(l->getContentSize() / 2 + ccp(0, -l->getContentSize().height / 2 + 23));
            l->addChild(cancel);

            auto search = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"), this, menu_selector(LoadMacroPopup::onSearch));
            //l->addChildAtPosition(search, Anchor::TopRight, ccp(-20, -50));

            auto info = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(LoadMacroPopup::onMacroInfo));
            l->addChildAtPosition(info, Anchor::TopRight, ccp(-16, -18));

            auto bg = CCLayerColor::create(ccc4(0, 0, 0, 50));
            bg->ignoreAnchorPointForPosition(false);
            l->addChildAtPosition(bg, Anchor::Center, ccp(0, 10));

            scroll = ScrollLayer::create(ccp(320, 175));
            scroll->setAnchorPoint(ccp(0, 0));
            l->addChildAtPosition(scroll, Anchor::Center, ccp(0, 10) - scroll->getContentSize() / 2);

            bg->setContentSize(scroll->getContentSize() + ccp(5, 5));
            
            auto content = scroll->m_contentLayer;

            auto path = Mod::get()->getConfigDir() / "macros";

            if (!utils::file::createDirectoryAll(path).isOk())
                return false;

            int tMacroCount = 0;
            int macroCount = 0;

            for (auto& dir : ghc::filesystem::directory_iterator(path))
                tMacroCount++;

            float totSize = std::max((35 * tMacroCount), 175);

            if (totSize < 200)
            {
                scroll->m_peekLimitTop = 0;
                scroll->m_peekLimitBottom = 0;
            }

            for (auto& dir : ghc::filesystem::directory_iterator(path))
            {
                if (!dir.is_directory())
                {
                    auto cell = MacroCell::createWithUnloadedMacro(dir.path().filename().stem().string(), macroCount);

                    if (cell)
                    {
                        cell->setPosition(ccp(0, totSize - (35 * macroCount) - 35));

                        content->addChild(cell);

                        macroCount++;
                    }
                }
            }

            content->setContentSize(ccp(320, totSize));
            scroll->moveToTop();

            auto openFolder = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"), this, menu_selector(LoadMacroPopup::onFolder));
            l->addChildAtPosition(openFolder, Anchor::BottomLeft, ccp(30, 30));

            auto refresh = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"), this, menu_selector(LoadMacroPopup::onRefresh));
            refresh->m_baseScale = 0.7f;
            refresh->setScale(refresh->m_baseScale);
            l->addChildAtPosition(refresh, Anchor::BottomRight, ccp(-25, 25));

            error = CCLabelBMFont::create("Nothing here yet :(", "bigFont.fnt");
            error->setScale(0.625f);
            error->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            if (macroCount == 0)
                l->addChildAtPosition(error, Anchor::Center, ccp(0, 10));

            as<CCNodeRGBA*>(error->getChildren()->objectAtIndex(17))->setColor(ccc3(94, 168, 234));
            as<CCNodeRGBA*>(error->getChildren()->objectAtIndex(18))->setColor(ccc3(94, 168, 234));

            this->addChild(l);

            //l->setScale(0.1f);
            //l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1))); 

            handleTouchPriority(this);
    
            return true;
        }

        static LoadMacroPopup* create()
        {
            LoadMacroPopup* pRet = new LoadMacroPopup();
            if (pRet && pRet->init()) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }

        static LoadMacroPopup* addToScene()
        {
            auto pRet = LoadMacroPopup::create();

            CCScene::get()->addChild(pRet, 99999);

            return pRet;
        }
};
