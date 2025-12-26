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
    loadedJson = file::readJson(path).unwrapOr("{ }");
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
                auto lbl = AdvLabelBMFont::createWithString("boobs", font.c_str());
                lbl->setScale(0);
                lbl->setID("THIS_IS_REQUIRED_FOR_LANGUAGE_PRELOADING"_spr);
                CCScene::get()->addChild(lbl);

                layer->removeFromParent();

                switchFinished();
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

std::string LocalisationManager::getLocalisedString(std::string id)
{
    // blank char to force the alt font, since its hard to read pusab
    auto errorStr = fmt::format("{}", id);

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