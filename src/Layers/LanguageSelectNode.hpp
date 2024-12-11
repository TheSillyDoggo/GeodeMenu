#pragma once

#include <Geode/Geode.hpp>
#include "TranslationCreditsLayer.hpp"

using namespace geode::prelude;

class LanguageSelectNode : public CCLayer
{
    public:
        CCMenu* node;
        TranslationCreditsLayer* layer;
        int page = 0;
        int changedBy = 0;

        bool init();

        void goToPage(int page);

        virtual void keyBackClicked();
        void onSubmit(CCObject* sender);

        void onLeft(CCObject* sender);
        void onRight(CCObject* sender);

        CREATE_FUNC(LanguageSelectNode);
        static LanguageSelectNode* addToScene();
};