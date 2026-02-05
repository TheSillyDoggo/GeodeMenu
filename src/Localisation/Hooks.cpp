#include "Hooks.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "LocalisationManager.hpp"
#include <TTFCache.hpp>

$on_mod (Loaded)
{
    LocalisationManager::get();
};

class $modify (LoadingLayer)
{
    struct Fields
    {
        int loadCount = 0;
    };

    void loadAssets()
    {
        LoadingLayer::loadAssets();

        auto font = LocalisationManager::get()->getAltFont();
        auto lang = LocalisationManager::get()->getCurrentLang();

        if (!font.empty())
        {
            if (m_fields->loadCount == 1)
            {
                if (!CCTextureCache::get()->textureForKey(utils::string::replace(font, ".fnt", ".png").c_str()))
                {
                    if (auto lbl = typeinfo_cast<CCLabelBMFont*>(getChildByID("geode-small-label")))
                        lbl->setString("QOLMod: Loading language font");

                    if (lang->getTrueTypeFallback())
                    {
                        Loader::get()->queueInMainThread([this]
                        {
                            TTFCache::get()->preloadTextures();
                        });
                    }
                    else
                    {
                        Loader::get()->queueInMainThread([this, font]
                        {
                            auto lbl = AdvLabelBMFont::createWithString("boobs", font.c_str());
                            lbl->setScale(0);
                            lbl->setID("THIS_IS_REQUIRED_FOR_LANGUAGE_PRELOADING"_spr);
                            CCScene::get()->addChild(lbl);
                        });
                    }
                }
            }
        }

        m_fields->loadCount++;
    }
};