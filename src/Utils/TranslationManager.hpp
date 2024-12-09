#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class TranslationManager
{
    private:
        std::unordered_map<std::string, std::string> translatedTexts;
    
    public:
        static TranslationManager* get();

        void loadTranslationFromJson(matjson::Value object);
        std::string getTranslatedString(std::string engText);
};