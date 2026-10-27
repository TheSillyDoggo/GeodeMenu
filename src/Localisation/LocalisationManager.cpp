#include "LocalisationManager.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../GUI/AndroidUI.hpp"
#include <FastBMFontConfig.hpp>
#include <asp/fs.hpp>
#include <asp/iter.hpp>

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

    asp::fs::Path path = Mod::get()->getResourcesDir() / Mod::get()->getSavedValue<std::string>("loaded-localisation-file", "en-AU.json");

    if (path.exists())
    {
        LocalisationManager::get()->loadLocalisationFile(path);
    }
    else
    {
        LocalisationManager::get()->loadLocalisationFile(Mod::get()->getResourcesDir() / "en-AU.json");
    }
}

void LocalisationManager::loadLocalisationFile(asp::fs::PathView path)
{
    currentLang = nullptr;

    if (languages.contains(path))
        currentLang = languages[path];

    currentPath = path;
}

asp::fs::PathView LocalisationManager::getCurrentLoadedFile()
{
    return currentPath;
}

void LocalisationManager::switchFinished()
{
    if (AndroidUI::get())
    {
        AndroidUI::get()->close();
        AndroidUI::addToScene()->playAnimation(qolmod::MenuAnimation::None);
    }
}

void LocalisationManager::switchLocalisationWithUIPath(asp::fs::PathView path)
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
                    FastBMFontConfig::quickLoad(font.c_str());

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

CLanguage* LocalisationManager::languageForPath(asp::fs::PathView path)
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

    auto iter = asp::fs::iterDir(Mod::get()->getResourcesDir()).unwrap();
    for (auto result : iter)
    {
        auto entry = result.unwrap();

        if (!entry.isFile())
            continue;

        if (entry.path.extension().has_value() && entry.path.extension().value() == "json")
        {
            files.push_back(entry.path.string());
        }
    }

    return files;
}

std::vector<asp::fs::Path> LocalisationManager::getAllLanguageFilesPath()
{
    std::vector<asp::fs::Path> files = {};

    auto iter = asp::fs::iterDir(Mod::get()->getResourcesDir()).unwrap();
    for (auto result : iter)
    {
        auto entry = result.unwrap();

        if (!entry.isFile())
            continue;

        if (entry.path.extension().has_value() && entry.path.extension().value() == "json")
        {
            files.push_back(entry.path);
        }
    }

    return files;
}

CLanguage* LocalisationManager::getCurrentLang()
{
    return currentLang ? currentLang : placeholder;
}

#include "ShapingEngine.hpp"
#include <Utils.hpp>

std::string LocalisationManager::reshapeArabicString(std::string str)
{
#ifdef GEODE_IS_MOBILE
    return str; // mobile renders arabic correctly with CCLabelTTF
#endif

    auto ws = qolmod::utils::toWideString(str);
    
    return ShapingEngine::render(ws, true);
}

void LocalisationManager::switchTempPath(asp::fs::PathView path)
{
    if (languages.contains(path))
        CC_SAFE_DELETE(languages[path]);

    auto data = file::readJson(path);

    if (data.isOk())
    {
        auto lang = CLanguage::createWithJSON(data.unwrap());

        if (languages.contains(path))
            languages[path] = lang;
        else
            languages.emplace(path, lang);
    }

    switchLocalisationWithUIPath(path);
}