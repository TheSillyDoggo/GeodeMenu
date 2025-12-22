#include "AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"

CCLabelTTFCache* CCLabelTTFCache::get()
{
    static CCLabelTTFCache* instance = nullptr;

    if (!instance)
    {
        instance = new CCLabelTTFCache();

        instance->label = CCLabelTTF::create("", "Arial.ttf", 32);
    }

    return instance;
}

CCTexture2D* CCLabelTTFCache::getTexture(std::string text)
{
    if (!stringsCached.contains(text))
        addTexture(text);

    return stringsCached[text];
}

void CCLabelTTFCache::addTexture(std::string text)
{
    if (stringsCached.contains(text))
        return;

    CCTexture2D* tex = new CCTexture2D();

    std::string fontName = "Arial.ttf";

    tex->initWithString(text.c_str(),
        fontName.c_str(),
        32 * CC_CONTENT_SCALE_FACTOR(),
        CC_SIZE_POINTS_TO_PIXELS(CCSizeZero),
        CCTextAlignment::kCCTextAlignmentLeft,
        kCCVerticalTextAlignmentCenter);

    stringsCached.emplace(text, tex);
}

void CCLabelTTFCache::flushTextures()
{
    for (auto texture : stringsCached)
    {
        CC_SAFE_DELETE(texture.second);
    }

    stringsCached.clear();
    assetsPreloaded = false;

    log::warn("CCLabelTTFCache - flushed textures");
}

void CCLabelTTFCache::preloadTextures()
{
    if (assetsPreloaded)
        return;

    for (auto obj : LocalisationManager::get()->getLoadedJson())
    {
        if (obj.isObject())
        {
            for (auto str : obj)
            {
                if (str.isString())
                {
                    addTexture(str.asString().unwrapOr(""));
                    continue;
                }

                if (str.isObject())
                {
                    for (auto str2 : str)
                    {
                        if (str2.isString())
                        {
                            addTexture(str2.asString().unwrapOr(""));
                            continue;
                        }
                    }
                }
            }
        }
    }

    assetsPreloaded = true;
    log::warn("CCLabelTTFCache - cached textures");
}