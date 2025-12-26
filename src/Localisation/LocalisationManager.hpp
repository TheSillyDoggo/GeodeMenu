#pragma once

#include <Geode/Geode.hpp>

class LocalisationManager
{
    protected:
        matjson::Value loadedJson;

        void loadLocalisationFile(std::filesystem::path path);
        void setup();

    public:
        static LocalisationManager* get();

        void switchLocalisationWithUIPath(std::filesystem::path path);
        void switchLocalisationWithUI(std::string file);

        void switchFinished();

        const matjson::Value& getLoadedJson();
        std::string getAltFont();
        std::string getLocalisedString(std::string id);
};