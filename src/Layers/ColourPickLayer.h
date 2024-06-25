/*#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Module.h"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

class ColourPickLayer : public SillyBaseLayer, public ColorPickPopupDelegate
{
    public:
        int icon;
        ccColor3B startFade;
        ccColor3B endFade;
        CCScale9Sprite* spr1;
        CCScale9Sprite* spr2;

        void onClose(CCObject* sender);

        virtual void customSetup();

        static ColourPickLayer* create(int icon)
        {
            IconOptionsLayer* pRet = new ColourPickLayer();
            
            pRet->icon = icon;

            if (pRet && pRet->initWithSizeAndName(ccp(350, 180), "Edit ")) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }

        static ColourPickLayer* addToScene(int icon)
        {
            auto pRet = ColourPickLayer::create(icon);

            CCScene::get()->addChild(pRet, 99999);

            return pRet;
        }

        virtual void updateColor(cocos2d::ccColor4B const& color);
};
*/