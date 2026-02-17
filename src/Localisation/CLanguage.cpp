#include "CLanguage.hpp"

using namespace geode::prelude;

CLanguage* CLanguage::createWithJSON(matjson::Value value)
{
    auto pRet = new CLanguage();

    pRet->displayNameEnglish = value["display_name_english"].asString().unwrapOr("Error");
    pRet->displayNameNative = value["display_name_native"].asString().unwrapOr("Error");
    pRet->font = value["font"].asString().unwrapOr("notosans.fnt");
    pRet->useTrueTypeFallback = value["use-true-type-fallback"].asBool().unwrapOr(false);
    pRet->splitDescByEveryChar = value["split-desc-by-every-char"].asBool().unwrapOr(false);
    pRet->dontTouchDescriptions = value["dont-split-descriptions"].asBool().unwrapOr(false);

    std::string usage = value["alt-font-usage"].asString().unwrapOr("Auto");

    if (usage == "Forced")
        pRet->fontUsageMode = 0;

    if (usage == "Auto")
        pRet->fontUsageMode = 1;

    if (usage == "None")
        pRet->fontUsageMode = 2;

    if (value.contains("strings") && value["strings"].isObject())
    {
        pRet->loadFromObject(value["strings"], "");
    }

    if (value.contains("contributors") && value["contributors"].isArray())
    {
        for (auto contributor : value["contributors"].asArray().unwrap())
        {
            if (!contributor.isObject())
                continue;
            
            pRet->contributors.push_back(CLanguageContributor::createWithJSON(contributor));
        }
    }

    return pRet;
}

CLanguageContributor CLanguageContributor::createWithJSON(matjson::Value value)
{
    CLanguageContributor pRet;

    pRet.username = value["username"].asString().unwrapOr("");
    pRet.accountID = value["account-id"].asInt().unwrapOr(1);
    pRet.iconID = value["icon-id"].asInt().unwrapOr(1);
    pRet.primaryCol = value["primary-col"].asInt().unwrapOr(1);
    pRet.secondaryCol = value["secondary-col"].asInt().unwrapOr(1);
    pRet.glowCol = value["glow-col"].asInt().unwrapOr(1);
    pRet.glowEnabled = value["glow-enabled"].asBool().unwrapOr(true);

    return pRet;
}

void CLanguage::loadFromObject(matjson::Value object, std::string stem)
{
    for (auto obj : object)
    {
        if (obj.isString())
        {
            strings.emplace(fmt::format("{}{}", stem, obj.getKey().value_or("")), obj.asString().unwrapOr(""));
        }

        if (obj.isObject())
        {
            loadFromObject(obj, fmt::format("{}{}/", stem, obj.getKey().value_or("")));
        }
    }
}

int CLanguage::getStringCount()
{
    return strings.size();
}

bool CLanguage::containsKey(std::string key)
{
    return strings.contains(key);
}

const std::string& CLanguage::stringForKey(const std::string& key)
{
    if (!containsKey(key))
        return key;

    return strings[key];
}

const std::unordered_map<std::string, std::string>& CLanguage::getStrings()
{
    return strings;
}

const std::vector<CLanguageContributor>& CLanguage::getContributors()
{
    return contributors;
}

const std::string& CLanguage::getEnglishName()
{
    return displayNameEnglish;
}

const std::string& CLanguage::getNativeName()
{
    return displayNameNative;
}

const std::string CLanguage::getFont()
{
    if (font.empty() || font == "chatFont.fnt")
        return "chatFont.fnt";
    else
        return fmt::format("thesillydoggo.qolmod/{}", font);
}

const int& CLanguage::getFontUsageMode()
{
    return fontUsageMode;
}

const bool& CLanguage::getTrueTypeFallback()
{
    return useTrueTypeFallback;
}

const bool& CLanguage::getSplitEachChar()
{
    return splitDescByEveryChar;
}

const bool& CLanguage::getDontTouchDescriptions()
{
    return dontTouchDescriptions;
}