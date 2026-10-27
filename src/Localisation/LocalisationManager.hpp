#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

#include "CLanguage.hpp"

class LocalisationManager
{
    protected:
        std::unordered_map<asp::fs::PathView, CLanguage*> languages = {};
        CLanguage* currentLang = nullptr;
        CLanguage* placeholder = nullptr;
        asp::fs::Path currentPath;

        void loadLocalisationFile(asp::fs::PathView path);
        void setup();

    public:
        static LocalisationManager* get();

        void switchTempPath(asp::fs::PathView path);
        void switchLocalisationWithUIPath(asp::fs::PathView path);
        void switchLocalisationWithUI(std::string file);

        void switchFinished();

        AdvLabelTTFUsage getDefaultTTFUsage();

        std::vector<std::string> getAllLanguageFiles();
        std::vector<asp::fs::Path> getAllLanguageFilesPath();

        asp::fs::PathView getCurrentLoadedFile();
        std::string getAltFont();
        std::string getLocalisedString(std::string id);
        std::string reshapeArabicString(std::string str);

        CLanguage* languageForPath(asp::fs::PathView path);
        CLanguage* getCurrentLang();
};