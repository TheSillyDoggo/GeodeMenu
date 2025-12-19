#include "LocalizationManager.hpp"

using namespace geode::prelude;

LocalizationManager* LocalizationManager::get()
{
    static LocalizationManager* instance = nullptr;

    if (!instance)
        instance = new LocalizationManager();

    return instance;
}

void LocalizationManager::loadLocalizationFile(std::filesystem::path path)
{
    loadedJson = file::readJson(path).unwrapOr("{ }");
}

std::string LocalizationManager::getLocalizedString(std::string id)
{
    // makes it more obvious that theres an error i think
    auto errorStr = utils::string::toUpper(id);

    auto splits = utils::string::split(id, "/");

    if (splits.size() > 1)
    {
        if (loadedJson.contains(splits[0]) && loadedJson[splits[0]].isObject())
        {
            if (loadedJson[splits[0]].contains(splits[1]))
            {
                return loadedJson[splits[0]][splits[1]].asString().unwrapOr(errorStr);
            }
        }
    }

    return errorStr;
}

$on_mod(Loaded)
{
    auto path = Mod::get()->getResourcesDir() / "en-AU.json";

    if (std::filesystem::exists(path))
    {
        LocalizationManager::get()->loadLocalizationFile(path);
        // TranslationManager::get()->loadTranslationFromJson(file::readJson(path).unwrapOr("{ }"));
    }
    else
    {
        // TranslationManager::get()->unloadTranslation();
    }
};