#pragma once

#include <Geode/Geode.hpp>

class LocalisationManager
{
    protected:
        matjson::Value loadedJson;

    public:
        static LocalisationManager* get();

        void loadLocalisationFile(std::filesystem::path path);

        const matjson::Value& getLoadedJson();
        std::string getAltFont();
        std::string getLocalisedString(std::string id);
};