#include "LocalisationManager.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../GUI/AndroidUI.hpp"

using namespace geode::prelude;

LocalisationManager* LocalisationManager::get()
{
    static LocalisationManager* instance = nullptr;

    if (!instance)
    {
        instance = new LocalisationManager();
        instance->setup();
    }

    return instance;
}

void LocalisationManager::setup()
{
    auto path = Mod::get()->getResourcesDir() / Mod::get()->getSavedValue<std::string>("loaded-localisation-file", "en-AU.json");

    if (std::filesystem::exists(path))
    {
        LocalisationManager::get()->loadLocalisationFile(path);
    }
    else
    {
        LocalisationManager::get()->loadLocalisationFile(Mod::get()->getResourcesDir() / "en-AU.json");
    }
}

void LocalisationManager::loadLocalisationFile(std::filesystem::path path)
{
    loadedJson = getCachedFile(path);
    currentPath = path;
}

matjson::Value LocalisationManager::getCachedFile(std::filesystem::path path)
{
    if (!loadedJsons.contains(path))
        loadedJsons.emplace(path, file::readJson(path).unwrapOr("{ }"));

    return loadedJsons[path];
}

std::filesystem::path LocalisationManager::getCurrentLoadedFile()
{
    return currentPath;
}

void LocalisationManager::switchFinished()
{
    if (AndroidUI::get())
    {
        AndroidUI::get()->close();
        AndroidUI::addToScene()->runAnimation(MenuAnimation::None);
    }
}

void LocalisationManager::switchLocalisationWithUIPath(std::filesystem::path path)
{
    loadLocalisationFile(path);
    auto font = LocalisationManager::get()->getAltFont();

    if (!font.empty() && !CCTextureCache::get()->textureForKey(utils::string::replace(font, ".fnt", ".png").c_str()))
    {
        auto layer = CCLayerColor::create(ccc4(0, 0, 0, 150));

        auto lbl = CCLabelBMFont::create("Loading language font", "bigFont.fnt");
        lbl->setScale(0.65f);
        lbl->setPosition(layer->getContentSize() / 2);

        layer->addChild(lbl);
        CCScene::get()->addChild(layer, CCScene::get()->getHighestChildZ() + 2);
        
        Loader::get()->queueInMainThread([this, layer, path, font]
        {
            Loader::get()->queueInMainThread([this, layer, path, font]
            {
                Loader::get()->queueInMainThread([this, layer, path, font]
                {
                    auto lbl = AdvLabelBMFont::createWithString("boobs", font.c_str());
                    lbl->setScale(0);
                    lbl->setID("THIS_IS_REQUIRED_FOR_LANGUAGE_PRELOADING"_spr);
                    CCScene::get()->addChild(lbl);

                    layer->removeFromParent();

                    switchFinished();
                });
            });
        });

        return;
    }

    switchFinished();
}

void LocalisationManager::switchLocalisationWithUI(std::string file)
{
    Mod::get()->setSavedValue<std::string>("loaded-localisation-file", file);

    switchLocalisationWithUIPath(Mod::get()->getResourcesDir() / file);
}

AdvLabelTTFUsage LocalisationManager::getDefaultTTFUsage()
{
    if (loadedJson.contains("alt-font-usage") && loadedJson["alt-font-usage"].isString())
    {
        auto str = utils::string::toLower(loadedJson["alt-font-usage"].asString().unwrapOr(""));

        if (str == "auto")
            return AdvLabelTTFUsage::Auto;

        if (str == "forced")
            return AdvLabelTTFUsage::Forced;

        if (str == "none")
            return AdvLabelTTFUsage::None;
    }
    
    return AdvLabelTTFUsage::Auto;
}

std::string LocalisationManager::getLocalisedString(std::string id)
{
    auto errorStr = fmt::format("<ttf>{}", id);
    auto splits = utils::string::split(id, "/");

    if (loadedJson.contains("strings") && loadedJson["strings"].isObject())
    {
        auto strings = loadedJson["strings"];

        if (splits.size() > 1)
        {
            if (strings.contains(splits[0]) && strings[splits[0]].isObject())
            {
                auto combined = id.substr(splits[0].size() + 1);

                if (strings[splits[0]].contains(combined) && strings[splits[0]][combined].isString())
                {
                    return strings[splits[0]][combined].asString().unwrapOr(errorStr);
                }

                if (splits.size() > 2)
                {
                    if (strings[splits[0]].contains(splits[1]) && strings[splits[0]][splits[1]].isObject())
                    {
                        if (strings[splits[0]][splits[1]].contains(splits[2]))
                        {
                            if (strings[splits[0]][splits[1]][splits[2]].isString())
                            {
                                return strings[splits[0]][splits[1]][splits[2]].asString().unwrapOr(errorStr);
                            }
                        }
                    }
                }
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

std::vector<std::string> LocalisationManager::getAllLanguageFiles()
{
    std::vector<std::string> files = {};

    for (auto file : std::filesystem::directory_iterator(Mod::get()->getResourcesDir()))
    {
        auto p = file.path().filename();

        if (p.has_extension() && p.extension().string() == ".json")
        {
            files.push_back(file.path().filename().string());
        }
    }

    return files;
}