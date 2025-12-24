#include "LocalisationManager.hpp"

using namespace geode::prelude;

LocalisationManager* LocalisationManager::get()
{
    static LocalisationManager* instance = nullptr;

    if (!instance)
        instance = new LocalisationManager();

    return instance;
}

void LocalisationManager::loadLocalisationFile(std::filesystem::path path)
{
    loadedJson = file::readJson(path).unwrapOr("{ }");

    auto cache = CCTextureCache::get();
    auto font = getAltFont();
    if (!font.empty())
    {
        CCTextureCache::get()->addImage(utils::string::replace(font, ".fnt", ".png").c_str(), false);

        // this is the best way to preload the font config
        auto lbl = CCLabelBMFont::create("boobs", font.c_str());
        lbl->setScale(0);
        lbl->setID("THIS_IS_REQUIRED_FOR_LANGUAGE_PRELOADING"_spr);
        CCScene::get()->addChild(lbl);
    }
}

std::string LocalisationManager::getLocalisedString(std::string id)
{
    // blank char to force the alt font, since its hard to read pusab
    auto errorStr = fmt::format("⠀{}", id);

    auto splits = utils::string::split(id, "/");

    if (loadedJson.contains("strings") && loadedJson["strings"].isObject())
    {
        auto strings = loadedJson["strings"];

        if (splits.size() > 1)
        {
            if (strings.contains(splits[0]) && strings[splits[0]].isObject())
            {
                auto combined = id.substr(splits[0].size() + 1);

                if (strings[splits[0]].contains(combined))
                {
                    return strings[splits[0]][combined].asString().unwrapOr(errorStr);
                }

                /*if (strings[splits[0]].contains(splits[1]))
                {
                    return strings[splits[0]][splits[1]].asString().unwrapOr(errorStr);
                }*/
            }
        }
    }

    return errorStr;
}

const matjson::Value& LocalisationManager::getLoadedJson()
{
    return loadedJson;
}

std::string LocalisationManager::getAltFont()
{
    if (loadedJson.contains("font") && loadedJson["font"].isString())
    {
        if (loadedJson["font"].asString().unwrapOr("") == "chatFont.fnt")
            return "chatFont.fnt";
        else
            return fmt::format("{}{}", ""_spr, loadedJson["font"].asString().unwrapOr(""));
    }

    return "";
}