#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LanguageNode : public CCNode
{
    protected:
        std::string lang = "en-AU.json";
        matjson::Value json = {};
        
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
        int getStringCount(matjson::Value value);
        std::unordered_map<std::string, std::string> getStrings(matjson::Value value);

        virtual void visit();

        bool init(std::string lang);
};