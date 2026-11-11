#pragma once

#include <Geode/Geode.hpp>
#include "../CLanguage.hpp"

class LanguageNode : public cocos2d::CCNode
{
    protected:
        std::string lang = "en-AU.json";
        CLanguage* language = nullptr;
        
        geode::NineSlice* bg;
        geode::Label* percentLabel;
        geode::Label* enName;
        cocos2d::CCLabelTTF* nativeName;
        cocos2d::CCMenu* useMenu = nullptr;

    public:
        static LanguageNode* create(std::string lang);

        void onSelectLanguage(cocos2d::CCObject* sender);
        void onViewCredits(cocos2d::CCObject* sender);
        void onMissingTranslations(cocos2d::CCObject* sender);

        float getPercentageComplete();

        virtual void visit();

        bool init(std::string lang);
};