#include "TTFCache.hpp"
#include <LocalisationManager.hpp>

using namespace geode::prelude;

TTFCache* TTFCache::get()
{
    static TTFCache* instance = nullptr;

    if (!instance)
    {
        instance = new TTFCache();
    }

    return instance;
}

CCTexture2D* TTFCache::getTexture(std::string text)
{
    text = LocalisationManager::get()->reshapeArabicString(text);

    if (!stringsCached.contains(text))
        addTexture(text);

    return stringsCached[text];
}

void TTFCache::addTexture(std::string text)
{
    if (stringsCached.contains(text))
        return;

    #ifndef GEODE_IS_IOS

    CCTexture2D* tex = new CCTexture2D();

    std::string fontName = "Arial.ttf";

    tex->initWithString(text.c_str(),
        fontName.c_str(),
        16 * CC_CONTENT_SCALE_FACTOR(),
        CC_SIZE_POINTS_TO_PIXELS(CCSizeZero),
        CCTextAlignment::kCCTextAlignmentLeft,
        kCCVerticalTextAlignmentCenter);

    stringsCached.emplace(text, tex);

    #endif
}

void TTFCache::flushTextures()
{
    for (auto texture : stringsCached)
    {
        CC_SAFE_DELETE(texture.second);
    }

    stringsCached.clear();
    // assetsPreloaded = false;

    log::warn("TTFCache - flushed textures");
}

void TTFCache::preloadTextures()
{
    /*if (assetsPreloaded)
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
    log::warn("TTFCache - cached textures");*/
}