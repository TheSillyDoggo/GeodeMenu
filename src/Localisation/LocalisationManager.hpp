#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

#include "CLanguage.hpp"

class LocalisationManager
{
    protected:
        std::unordered_map<asp::fs::Path, CLanguage*> languages = {};
        CLanguage* currentLang = nullptr;
        CLanguage* placeholder = nullptr;
        asp::fs::Path currentPath;

        void loadLocalisationFile(asp::fs::Path path);
        void setup();

    public:
        static LocalisationManager* get();

        void switchTempPath(asp::fs::Path path);
        void switchLocalisationWithUIPath(asp::fs::Path path);
        void switchLocalisationWithUI(std::string file);

        void switchFinished();

        AdvLabelTTFUsage getDefaultTTFUsage();

        std::vector<std::string> getAllLanguageFiles();
        std::vector<asp::fs::Path> getAllLanguageFilesPath();

        asp::fs::Path getCurrentLoadedFile();
        std::string getAltFont();
        std::string getLocalisedString(std::string id);

        CLanguage* languageForPath(asp::fs::Path path);
        CLanguage* getCurrentLang();
};