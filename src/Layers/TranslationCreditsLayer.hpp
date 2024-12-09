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
        GJGroundLayer* ground;
        CCSprite* background;

        virtual void customSetup();

        void onPlayerProfile(CCObject* sender);
        void onKill(CCObject* sender);

        static TranslationCreditsLayer* create(matjson::Value language);
        static TranslationCreditsLayer* addToScene(matjson::Value language);
};