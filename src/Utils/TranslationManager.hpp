#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class TranslationManager
{
    private:
        std::unordered_map<std::string, std::string> translatedTexts;
        std::string currentLanguage;
        bool languageLoaded = false;
    
    public:
        static TranslationManager* get();

        void unloadTranslation();
        void loadTranslationFromJson(matjson::Value object);
        std::string getTranslatedString(std::string engText);

        bool isLanguageLoaded();
        std::string getLoadedLanguage();
};