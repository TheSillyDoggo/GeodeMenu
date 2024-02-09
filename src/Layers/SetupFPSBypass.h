#include "../Client/Dropdown.h"
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SetupFPSBypass : public FLAlertLayer
{
    public:
        geode::InputNode* inp = nullptr;
        CCMenuItemToggler* enb = nullptr;

        void onClose(CCObject*)
        {
            if (GameManager::get()->getGameVariable("0116") != enb->isToggled())
            {
                GameManager::get()->m_customFPSTarget = std::stof(inp->getString());
                GameManager::get()->setGameVariable("0116", enb->isToggled());
                GameManager::get()->setGameVariable("0030", !enb->isToggled());

                GameManager::get()->updateCustomFPS();
            }

            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
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

            auto title = CCLabelBMFont::create("Setup FPS Bypass", "bigFont.fnt");
            title->setPosition(l->getContentSize() / 2 + ccp(0, 90));
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChild(title);

            auto lb = CCLabelBMFont::create("FPS Bypass:", "bigFont.fnt");
            lb->setPosition(l->getContentSize() / 2 + ccp(-20, 30));
            lb->setScale(0.55f);
            lb->setAnchorPoint(ccp(1, 0.5f));
            l->addChild(lb);

            std::stringstream ss2;
            ss2 << GameManager::get()->m_customFPSTarget;

            inp = geode::InputNode::create(lb->getScaledContentSize().width, "FPS Value");
            inp->setPosition(l->getContentSize() / 2 + ccp(50, 30));
            inp->setString(ss2.str());
            inp->getInput()->setAllowedChars("098765431.");
            inp->getInput()->setMaxLabelLength(8);
            l->addChild(inp);

            auto lb2 = CCLabelBMFont::create("Enabled:", "bigFont.fnt");
            lb2->setPosition(l->getContentSize() / 2 + ccp(-20, -10));
            lb2->setScale(0.55f);
            lb2->setAnchorPoint(ccp(1, 0.5f));
            l->addChild(lb2);

            enb = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.8f);
            enb->setPosition(l->getContentSize() / 2 + ccp(20, -10));
            enb->toggle(GameManager::get()->getGameVariable("0116"));
            l->addChild(enb);

            auto ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SetupFPSBypass::onClose));
            ok->setPosition(l->getContentSize() / 2 + ccp(0, -82));
            l->addChild(ok);

            this->addChild(l);

            l->setScale(0.1f);
            l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1))); 
    
            return true;
        }

        static SetupFPSBypass* create()
        {
            SetupFPSBypass* pRet = new SetupFPSBypass();
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
            auto pRet = SetupFPSBypass::create();

            CCScene::get()->addChild(pRet, 99999);
        }
};