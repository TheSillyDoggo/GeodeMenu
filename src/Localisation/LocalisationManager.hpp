#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

class LocalisationManager
{
    protected:
        std::unordered_map<std::filesystem::path, matjson::Value> loadedJsons = {};
        matjson::Value loadedJson;
        std::filesystem::path currentPath;

        void loadLocalisationFile(std::filesystem::path path);
        void setup();

    public:
        static LocalisationManager* get();

        void switchLocalisationWithUIPath(std::filesystem::path path);
        void switchLocalisationWithUI(std::string file);

        void switchFinished();

        AdvLabelTTFUsage getDefaultTTFUsage();

        matjson::Value getCachedFile(std::filesystem::path path);
        std::filesystem::path getCurrentLoadedFile();

        const matjson::Value& getLoadedJson();
        std::string getAltFont();
        std::string getLocalisedString(std::string id);
};