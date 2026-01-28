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
    placeholder = CLanguage::createWithJSON("{}");

    for (auto file : getAllLanguageFilesPath())
    {
        auto data = file::readJson(file);

        if (data.isOk())
        {
            auto lang = CLanguage::createWithJSON(data.unwrap());

            languages.emplace(file, lang);
        }
    }

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
    currentLang = nullptr;

    if (languages.contains(path))
        currentLang = languages[path];

    currentPath = path;
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
    if (currentLang)
    {
        switch (currentLang->getFontUsageMode())
        {
            case 0:
                return AdvLabelTTFUsage::Forced;

            case 1:
                return AdvLabelTTFUsage::Auto;

            case 2:
                return AdvLabelTTFUsage::None;
        }
    }
    
    return AdvLabelTTFUsage::Auto;
}

std::string LocalisationManager::getLocalisedString(std::string id)
{
    auto errorStr = fmt::format("<ttf>{}", id);
    
    if (currentLang && currentLang->containsKey(id))
        return currentLang->stringForKey(id);

    return errorStr;
}

CLanguage* LocalisationManager::languageForPath(std::filesystem::path path)
{
    if (languages.contains(path))
        return languages[path];

    return placeholder;
}

std::string LocalisationManager::getAltFont()
{
    if (!currentLang)
        return "notosans.fnt"_spr;

    return currentLang->getFont();
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

std::vector<std::filesystem::path> LocalisationManager::getAllLanguageFilesPath()
{
    std::vector<std::filesystem::path> files = {};

    for (auto file : std::filesystem::directory_iterator(Mod::get()->getResourcesDir()))
    {
        auto p = file.path().filename();

        if (p.has_extension() && p.extension().string() == ".json")
        {
            files.push_back(file);
        }
    }

    return files;
}