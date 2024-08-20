#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Module.h"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

class ManageKeybindsLayer : public SillyBaseLayer
{
    public:

        virtual void customSetup();

        void onSet(CCObject* sender);
        void onDelete(CCObject* sender);

        static ManageKeybindsLayer* create();
        static ManageKeybindsLayer* addToScene()
        {
            auto pRet = ManageKeybindsLayer::create();

            CCScene::get()->addChild(pRet, 99999);

            return pRet;
        }
};
