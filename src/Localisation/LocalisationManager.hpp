#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

#include "CLanguage.hpp"

class LocalisationManager
{
    protected:
        std::unordered_map<std::filesystem::path, CLanguage*> languages = {};
        CLanguage* currentLang = nullptr;
        CLanguage* placeholder = nullptr;
        std::filesystem::path currentPath;

        void loadLocalisationFile(std::filesystem::path path);
        void setup();

    public:
        static LocalisationManager* get();

        void switchLocalisationWithUIPath(std::filesystem::path path);
        void switchLocalisationWithUI(std::string file);

        void switchFinished();

        AdvLabelTTFUsage getDefaultTTFUsage();

        std::vector<std::string> getAllLanguageFiles();
        std::vector<std::filesystem::path> getAllLanguageFilesPath();

        std::filesystem::path getCurrentLoadedFile();
        std::string getAltFont();
        std::string getLocalisedString(std::string id);

        CLanguage* languageForPath(std::filesystem::path path);
};