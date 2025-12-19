#pragma once

#include <Geode/Geode.hpp>

class LocalizationManager
{
    protected:
        matjson::Value loadedJson;

    public:
        static LocalizationManager* get();

        void loadLocalizationFile(std::filesystem::path path);

        std::string getLocalizedString(std::string id);
};