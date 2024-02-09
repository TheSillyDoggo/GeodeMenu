#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SetupFadeSetting : public FLAlertLayer
{
    public:
        int mode = -1;
        CCScale9Sprite* left = nullptr;
        CCScale9Sprite* right = nullptr;

        bool leftA = false;

        void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        void update(float dt)
        {
            std::stringstream fadeIn;
            fadeIn << "fadeColour1";
            fadeIn << mode;

            std::stringstream fadeOut;
            fadeOut << "fadeColour2";
            fadeOut << mode;

            ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn.str(), {0, 0, 0});
            ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut.str(), {255, 255, 255});

            left->setColor(in);
            right->setColor(out);
        }

        void onColour(CCObject* sender)
        {
            leftA = (static_cast<CCNode*>(sender)->getTag() == 0);

            std::stringstream fadeIn;
            fadeIn << "fadeColour1";
            fadeIn << mode;

            std::stringstream fadeOut;
            fadeOut << "fadeColour2";
            fadeOut << mode;

            ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn.str(), {0, 0, 0});
            ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut.str(), {255, 255, 255});

            //auto pop = ColorPickPopup::create((leftA ? in : out));
            //auto pop = geode::ColorPickPopup::create({255, 0, 0, 255}, false);
            //CCScene::get()->addChild(pop, this->getZOrder() + 1);
        }

        bool init(int mode)
        {
            if (!FLAlertLayer::init(0))
                return false;

            this->mode = mode;

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

            auto title = CCLabelBMFont::create("Setup Icon Fade\nCustomising speed and colours is not yet supported", "bigFont.fnt");
            title->setPosition(l->getContentSize() / 2 + ccp(0, 90));
            title->setOpacity(100);
            title->setScale(0.5f);
            l->addChild(title);

            auto ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SetupFadeSetting::onClose));
            ok->setPosition(l->getContentSize() / 2 + ccp(0, -82));
            l->addChild(ok);


            left = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            left->setContentSize(ccp(90, 35));
            auto leftBtn = CCMenuItemSpriteExtra::create(left, this, menu_selector(SetupFadeSetting::onColour));
            leftBtn->setEnabled(false);
            leftBtn->setTag(0);
            leftBtn->setPosition(l->getContentSize() / 2 + ccp(-80, -15));
            l->addChild(leftBtn);

            right = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            right->setContentSize(ccp(90, 35));
            auto rightBtn = CCMenuItemSpriteExtra::create(right, this, menu_selector(SetupFadeSetting::onColour));
            rightBtn->setEnabled(false);
            rightBtn->setTag(1);
            rightBtn->setPosition(l->getContentSize() / 2 + ccp(80, -15));
            l->addChild(rightBtn);

            this->addChild(l);

            l->setScale(0.1f);
            l->runAction(CCEaseElasticOut::create(CCScaleTo::create(1, 1)));       
            this->scheduleUpdate();     
    
            return true;
        }

        static SetupFadeSetting* create(int mode)
        {
            SetupFadeSetting* pRet = new SetupFadeSetting();
            if (pRet && pRet->init(mode)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }
};