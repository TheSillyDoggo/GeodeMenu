#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Module.h"
#include "../Hacks/SafeMode/SafeMode.hpp"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

class TranslationCreditsLayer : public SillyBaseLayer
{
    public:
        matjson::Value language;
        std::filesystem::path path;
        GJGroundLayer* ground;
        CCSprite* background;
        ButtonSprite* sprUse;
        CCMenuItemSpriteExtra* btnUse;

        virtual void customSetup();

        void onPlayerProfile(CCObject* sender);
        void onKill(CCObject* sender);

        void onUse(CCObject* sender);

        static TranslationCreditsLayer* create(matjson::Value language, std::filesystem::path path);
        static TranslationCreditsLayer* addToScene(matjson::Value language, std::filesystem::path path);
};