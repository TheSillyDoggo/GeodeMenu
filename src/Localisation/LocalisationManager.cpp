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

const matjson::Value& LocalisationManager::getLoadedJson()
{
    return loadedJson;
}

std::string LocalisationManager::getAltFont()
{
    if (loadedJson.contains("font") && loadedJson["font"].isString())
        return fmt::format("{}{}", ""_spr, loadedJson["font"].asString().unwrapOr(""));

    return "";
}