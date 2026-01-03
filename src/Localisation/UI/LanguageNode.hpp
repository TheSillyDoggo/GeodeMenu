#pragma once

#include <Geode/Geode.hpp>
#include "../CLanguage.hpp"

using namespace geode::prelude;

class LanguageNode : public CCNode
{
    protected:
        std::string lang = "en-AU.json";
        CLanguage* language = nullptr;
        
        CCScale9Sprite* bg;
        CCLabelBMFont* percentLabel;
        CCLabelBMFont* enName;
        CCLabelTTF* nativeName;
        CCMenu* useMenu = nullptr;

    public:
        static LanguageNode* create(std::string lang);

        void onSelectLanguage(CCObject* sender);
        void onViewCredits(CCObject* sender);
        void onMissingTranslations(CCObject* sender);

        float getPercentageComplete();

        virtual void visit();

        bool init(std::string lang);
};