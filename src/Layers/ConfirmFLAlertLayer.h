#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class ConfirmFLAlertLayer : public FLAlertLayer
{
    public:
        bool init(FLAlertLayerProtocol* delegate, char const* title, char const* confLabel, bool confEnabled, SEL_MenuHandler confChanged, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale)
        {
            if (!FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale))
                return false;

            auto l = getChildOfType<CCLayer>(this, 0);
            auto menu = getChildOfType<CCMenu>(l, 0);
            auto bg = getChildOfType<CCScale9Sprite>(l, 0);

            float inc = 20;

            bg->setContentHeight(bg->getContentHeight() + inc);
            getChildOfType<CCLabelBMFont>(l, 0)->setPositionY(getChildOfType<CCLabelBMFont>(l, 0)->getPositionY() + inc / 2);
            getChildOfType<TextArea>(l, 0)->setPositionY(getChildOfType<TextArea>(l, 0)->getPositionY() + inc / 2);
            menu->setPositionY(menu->getPositionY() - inc / 2);

            auto label = CCLabelBMFont::create(confLabel, "chatFont.fnt");
            label->setAnchorPoint(ccp(0, 0));
            label->limitLabelWidth(bg->getContentWidth() - 30 - 30, 0.8f, 0);
            label->setPosition(ccp(-(bg->getContentWidth() / 2) + 30 + 10, 23));
            menu->addChild(label);

            auto toggler = CCMenuItemToggler::createWithStandardSprites(this, confChanged, 0.55f);
            toggler->toggle(confEnabled);
            toggler->setPosition(label->getPosition() + ccp(-10 - 3, 10 - 3));
            menu->addChild(toggler);
    
            return true;
        }

        static ConfirmFLAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, char const* confLabel, bool confEnabled, SEL_MenuHandler confChanged, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale)
        {
            ConfirmFLAlertLayer* pRet = new ConfirmFLAlertLayer();
            if (pRet && pRet->init(delegate, title, confLabel, confEnabled, confChanged, desc, btn1, btn2, width, scroll, height, textScale)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }
};
