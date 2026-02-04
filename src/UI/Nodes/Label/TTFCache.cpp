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
    if (!stringsCached.contains(text))
        addTexture(text);

    return stringsCached[text];
}

void TTFCache::addTexture(std::string text)
{
    if (stringsCached.contains(text))
        return;

    CCTexture2D* tex = new CCTexture2D();
    auto text2 = LocalisationManager::get()->reshapeArabicString(text);

    std::string fontName = "Arial.ttf";

    tex->initWithString(text2.c_str(),
        fontName.c_str(),
        16 * CC_CONTENT_SCALE_FACTOR(),
        CC_SIZE_POINTS_TO_PIXELS(CCSizeZero),
        CCTextAlignment::kCCTextAlignmentLeft,
        kCCVerticalTextAlignmentCenter);

    stringsCached.emplace(text, tex);
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