#include <Geode/Geode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class SaveMacroPopup : public FLAlertLayer, TextInputDelegate
{
    public:
        geode::InputNode* inp = nullptr;
        CCLabelBMFont* errorLbl = nullptr;
        ButtonSprite* bs = nullptr;
        CCMenuItemSpriteExtra* ok = nullptr;

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        void onOk(CCObject*)
        {
            //log::info("n: {}", Client::GetModuleEnabled("noclip"));

            //Client::replay.author = GJAccountManager::get()->m_username;

            //std::vector<uint8_t> output = replay.exportData(true);
            //std::string str(output.begin(), output.end());

            //utils::file::writeString(Mod::get()->getConfigDir() / "macros" / inp->getString() / ".gdr", str.c_str());

            TextAlertPopup::create("a", 0.3f, 0.1f, 1, "unk");

            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
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

            inp = geode::InputNode::create(lb->getScaledContentSize().width + 150, "Macro Name (.gdr)");
            inp->setPosition(l->getContentSize() / 2 + ccp(0, 5));
            inp->getInput()->setAllowedChars("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNNM098765431");
            inp->getInput()->setMaxLabelLength(32);
            inp->getInput()->setDelegate(this);
            inp->getInput()->setID("IGNOREBYPASSES"_spr);
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
