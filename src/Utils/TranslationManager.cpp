#include "TranslationManager.hpp"

TranslationManager* TranslationManager::get()
{
    static TranslationManager* instance = nullptr;

    if (!instance)
        instance = new TranslationManager();

    return instance;
}

void TranslationManager::unloadTranslation()
{
    translatedTexts = {};
    currentLanguage = "";
    languageLoaded = false;
}

void TranslationManager::loadTranslationFromJson(matjson::Value object)
{
    unloadTranslation();

    if (!object.contains("strings"))
        return;

    auto strings = object["strings"];

    for (auto value : strings)
    {
        if (value.isString() && value.getKey().has_value())
        {
            translatedTexts.emplace(value.getKey().value(), value.asString().unwrapOr("Unknown unwrap error :c"));
        }
    }

    languageLoaded = true;
    currentLanguage = object["display_name_english"].asString().unwrapOr("ERROR");
}

std::string TranslationManager::getLoadedLanguage()
{
    return currentLanguage;
}

bool TranslationManager::isLanguageLoaded()
{
    return languageLoaded;
}

std::string TranslationManager::getTranslatedString(std::string engText)
{
    if (translatedTexts.contains(engText))
        return translatedTexts[engText];

    return engText;
}