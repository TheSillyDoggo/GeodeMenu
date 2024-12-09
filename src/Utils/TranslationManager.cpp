#include "TranslationManager.hpp"

TranslationManager* TranslationManager::get()
{
    static TranslationManager* instance = nullptr;

    if (!instance)
        instance = new TranslationManager();

    return instance;
}

void TranslationManager::loadTranslationFromJson(matjson::Value object)
{
    translatedTexts = {};

    for (auto value : object)
    {
        if (value.isString() && value.getKey().has_value())
        {
            translatedTexts.emplace(value.getKey().value(), value.asString().unwrapOr("Unknown unwrap error :c"));
        }
    }
}

std::string TranslationManager::getTranslatedString(std::string engText)
{
    if (translatedTexts.contains(engText))
        return translatedTexts[engText];

    return engText;
}