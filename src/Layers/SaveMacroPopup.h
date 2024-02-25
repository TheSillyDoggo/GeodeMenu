#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Client.h"

#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class SaveMacroPopup : public FLAlertLayer, TextInputDelegate
{
    public:
        geode::TextInput* inp = nullptr;
        CCLabelBMFont* errorLbl = nullptr;
        ButtonSprite* bs = nullptr;
        CCMenuItemSpriteExtra* ok = nullptr;

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        void onOk(CCObject*)
        {
            GJReplayManager::replay.author = GJAccountManager::get()->m_username;

            std::vector<uint8_t> output = GJReplayManager::replay.exportData(true);
            std::string str(output.begin(), output.end());

            auto p = Mod::get()->getConfigDir() / "macros" / (inp->getString() + ".gdr");

            log::info("s: {}", str);

            auto res = utils::file::writeString(p, str.c_str());

            CCScene::get()->addChild(TextAlertPopup::create("Successfully saved '" + inp->getString() + ".gdr'", 0.5f, 0.6f, 150, ""), 9999999);

            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        void onMacroInfo(CCObject*)
        {
            std::stringstream ss;
            ss << "Level Name: <ca>";
            ss << GJReplayManager::replay.levelInfo.name;
            ss << "</c>\n";
            
            ss << "Level ID: <cy>";
            ss << GJReplayManager::replay.levelInfo.id;
            ss << "</c>\n";
            
            ss << "LDM: <cg>";
            ss << GJReplayManager::replay.ldm;
            ss << "</c>\n";
            
            ss << "Inputs: <cb>";
            ss << GJReplayManager::replay.inputs.size();
            ss << "</c>\n";

            ss << "Macro Author: <cp>";
            ss << GJReplayManager::replay.author;
            ss << "</c>\n";

            ss << "Duration: <cl>";
            ss << GJReplayManager::replay.duration;
            ss << "</c>\n";

            ss << "Description: <cr>";
            ss << GJReplayManager::replay.description;
            //ss << "</c>\n";

            FLAlertLayer::create("Macro Info", ss.str(), "OK")->show();
        }

        virtual void textChanged(CCTextInputNode* p0)
        {
            errorLbl->setString("");
            bs->updateBGImage("GJ_button_01.png");
            ok->setEnabled(true);

            if (GJReplayManager::replay.inputs.size() == 0)
            {
                errorLbl->setString("Macro cannot be empty");
                errorLbl->limitLabelWidth(725*0.375, 1.0f, 0.1f);

                bs->updateBGImage("GJ_button_04.png");
                ok->setEnabled(false);

                return;
            }

            if (p0->getString().size() == 0)
            {
                errorLbl->setString("Macro name cannot be empty");
                errorLbl->limitLabelWidth(725*0.375, 1.0f, 0.1f);

                bs->updateBGImage("GJ_button_04.png");
                ok->setEnabled(false);

                return;
            }

            std::stringstream ss;
            ss << Mod::get()->getConfigDir().string();
            ss << "/macros/";
            ss << p0->getString().c_str();
            ss << ".gdr";

            log::info("path: {}", ss.str());

            if (CCFileUtils::get()->isFileExist(ss.str()))
            {
                errorLbl->setString("Macro already exists by that name");
                errorLbl->limitLabelWidth(725*0.375, 1.0f, 0.1f);

                bs->updateBGImage("GJ_button_04.png");
                ok->setEnabled(false);
            }
        }

        bool init()
        {
            GJReplayManager::replay.author = GJAccountManager::get()->m_username;


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

            auto title = CCLabelBMFont::create("Save Macro As", "bigFont.fnt");
            title->setPosition(l->getContentSize() / 2 + ccp(0, 90));
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChild(title);

            auto lb = CCLabelBMFont::create("Macro Name:", "bigFont.fnt");
            lb->setPosition(l->getContentSize() / 2 + ccp(0, 50));
            lb->setScale(0.55f);
            l->addChild(lb);

            inp = geode::TextInput::create(lb->getScaledContentSize().width + 150, "Macro Name (.gdr)");
            inp->setPosition(l->getContentSize() / 2 + ccp(0, 5));
            inp->getInputNode()->setAllowedChars("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNNM098765431 ");
            inp->getInputNode()->setMaxLabelLength(32);
            inp->getInputNode()->setDelegate(this);
            inp->getInputNode()->setID("IGNOREBYPASSES"_spr);
            l->addChild(inp);

            errorLbl = CCLabelBMFont::create(GJReplayManager::replay.inputs.size() == 0 ? "Macro cannot be empty" : "Macro name cannot be empty", "bigFont.fnt");
            errorLbl->setColor(ccc3(255, 0, 0));
            errorLbl->setOpacity(100);
            errorLbl->setPosition(l->getContentSize() / 2 + ccp(0, -25));
            errorLbl->limitLabelWidth(725*0.375, 1.0f, 0.1f);
            l->addChild(errorLbl);

            auto cancel = CCMenuItemSpriteExtra::create(ButtonSprite::create("Cancel"), this, menu_selector(SaveMacroPopup::onClose));
            cancel->setPosition(l->getContentSize() / 2 + ccp(-40, -82));
            l->addChild(cancel);

            bs = ButtonSprite::create("OK", "goldFont.fnt", "GJ_button_04.png");

            ok = CCMenuItemSpriteExtra::create(bs, this, menu_selector(SaveMacroPopup::onOk));
            ok->setPosition(l->getContentSize() / 2 + ccp(58, -82));
            ok->setEnabled(false);
            l->addChild(ok);

            auto info = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(SaveMacroPopup::onMacroInfo));
            l->addChildAtPosition(info, Anchor::TopRight, ccp(-10 - 6, -10 - 8));

            this->addChild(l);

            l->setScale(0.1f);
            l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1))); 
    
            return true;
        }

        static SaveMacroPopup* create()
        {
            SaveMacroPopup* pRet = new SaveMacroPopup();
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
            auto pRet = SaveMacroPopup::create();

            CCScene::get()->addChild(pRet, 99999);
        }
};
