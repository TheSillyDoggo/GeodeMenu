#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Client.h"
#include "../Utils/CCBlurLayer.hpp"
#include "../UI/QOLModMenuBG.hpp"

using namespace geode::prelude;

class SillyBaseLayer : public geode::Popup<>
{
    public:
        CCMenu* baseLayer = nullptr;
        CCMenuItemSpriteExtra* ok = nullptr;
        CCPoint size = CCPointZero;
        CCBlurLayer* blur = nullptr;
        std::string _title;
        CCLabelBMFont* title;
        bool createWithOK;
        bool animate;
        bool noBlur;
        QOLModMenuBG* bg;

        ~SillyBaseLayer();

        virtual void onClose(CCObject*)
        {
            this->removeFromParent();
        }

        virtual void keyBackClicked()
        {
            onClose(nullptr);
        }

        virtual void customSetup()
        {

        }

        virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
        virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
        virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
        virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);

        virtual bool setup();

        bool initWithSizeAndName(CCPoint size, std::string _title, bool createWithOK = true, bool animate = true, bool noBlur = false);
};
