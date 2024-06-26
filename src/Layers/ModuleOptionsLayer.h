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

        static ModuleOptionsLayer* create(Module* module)
        {
            ModuleOptionsLayer* pRet = new ModuleOptionsLayer();
            
            pRet->mod = module;

            if (pRet && pRet->initWithSizeAndName(ccp(350, std::ceil((std::max<int  >(module->options.size(), 3) - 1) / 2) * 35 + 110), module->name)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }

        static ModuleOptionsLayer* addToScene(Module* module)
        {
            auto pRet = ModuleOptionsLayer::create(module);

            CCScene::get()->addChild(pRet, 99999);

            return pRet;
        }
};
