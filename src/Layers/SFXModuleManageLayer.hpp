#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Types/SFXModule.hpp"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

class SFXModuleManageLayer : public SillyBaseLayer
{
    private:
        SFXModule* mod;
        ScrollLayer* scroll;
        TextArea* warning;
        Ref<SFXBrowser> browser = nullptr;

    public:
        virtual void customSetup();
        virtual void update(float);

        void updateList();

        void onPreviewSFX(CCObject* sender);
        void onRemoveSFX(CCObject* sender);
        void onNewSFX(CCObject* sender);
        void onFileSFX(CCObject* sender);

        static SFXModuleManageLayer* create(SFXModule* mod);
        static SFXModuleManageLayer* addToScene(SFXModule* mod)
        {
            auto pRet = SFXModuleManageLayer::create(mod);

            CCScene::get()->addChild(pRet, 99999);

            return pRet;
        }
};