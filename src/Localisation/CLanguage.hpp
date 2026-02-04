#pragma once

#include <Geode/Geode.hpp>

struct CLanguageContributor
{
    std::string username = "";
    int accountID = 80085;
    int iconID = 80085;
    int primaryCol = 80085;
    int secondaryCol = 80085;
    int glowCol = 80085;
    bool glowEnabled = false;

    static CLanguageContributor createWithJSON(matjson::Value value);
};

class CLanguage
{
    protected:
        std::unordered_map<std::string, std::string> strings = {};
        std::vector<CLanguageContributor> contributors = {};
        std::string displayNameEnglish = "";
        std::string displayNameNative = "";
        std::string font = "";
        int fontUsageMode = 0;
        bool useTrueTypeFallback = false;

        void loadFromObject(matjson::Value object, std::string stem = "");

    public:
        static CLanguage* createWithJSON(matjson::Value value);

        const std::unordered_map<std::string, std::string>& getStrings();
        const std::vector<CLanguageContributor>& getContributors();
        const std::string& getEnglishName();
        const std::string& getNativeName();
        const std::string getFont();
        const int& getFontUsageMode();
        const bool& getTrueTypeFallback();

        int getStringCount();
        bool containsKey(std::string key);
        const std::string& stringForKey(const std::string& key);
};