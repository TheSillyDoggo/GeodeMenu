#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Module.h"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

class ModuleOptionsLayer : public SillyBaseLayer
{
    public:
        Module* mod = nullptr;

        virtual void customSetup();

        void onInfoButton(CCObject*);

        static ModuleOptionsLayer* create(Module* module);

        static ModuleOptionsLayer* addToScene(Module* module)
        {
            auto pRet = ModuleOptionsLayer::create(module);

            CCScene::get()->addChild(pRet, 99999);

            return pRet;
        }
};
