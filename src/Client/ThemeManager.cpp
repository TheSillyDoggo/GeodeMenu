#include "ThemeManager.hpp"
#include <Geode/Geode.hpp>
#include <asp/fs.hpp>

using namespace geode::prelude;
using namespace qolmod;

ThemeManager* ThemeManager::get()
{
    static ThemeManager* instance = []{
        auto pRet = new ThemeManager();
        pRet->load();
        return pRet;
    }();

    return instance;
}

void ThemeManager::save()
{
    Mod::get()->setSavedValue<int>("theme", (int)background);
}

void ThemeManager::load()
{
    auto oldPath = Mod::get()->getConfigDir() / "ui-background.png";
    background = (BackgroundType)Mod::get()->getSavedValue<int>("theme", (int)background);

    if (!oldPath.exists())
    {
        auto path = Mod::get()->getSavedValue<std::string>("custom-background-path", "");

        if (!path.empty())
        {
            asp::fs::copy(path, oldPath);
            Mod::get()->setSavedValue<std::string>("custom-background-path", "");
        }
    }
}

MenuAnimation ThemeManager::getAnimation()
{
    return anim;
}

void ThemeManager::setAnimation(MenuAnimation animation)
{
    this->anim = animation;
    save();
}

BackgroundType ThemeManager::getBackground()
{
    return background;
}

void ThemeManager::setBackground(BackgroundType background)
{
    this->background = background;
    save();
}

void ThemeManager::setCustomSprite(asp::fs::PathView path)
{
    auto oldPath = Mod::get()->getConfigDir() / "ui-background.png";

    CCTextureCache::get()->removeTextureForKey(oldPath.c_str());
    if (path.empty() || !path.exists())
    {
        asp::fs::removeFile(oldPath);

        return;
    }

    asp::fs::copy(path, oldPath);
}

CCSprite* ThemeManager::createCustomSprite(bool returnDefault)
{
    auto path = Mod::get()->getConfigDir() / "ui-background.png";
    CCSprite* spr = nullptr;

    if (CCFileUtils::get()->isFileExist(path.c_str()))
        spr = CCSprite::create(path.c_str());

    if (spr)
    {
        if (spr->getUserObject("geode.texture-loader/fallback"))
            spr = nullptr;

        #if GEODE_COMP_GD_VERSION >= 22081
        if (spr && spr->isUsingFallback())
            spr = nullptr;
        #endif
    }

    if (!spr && returnDefault)
        spr = CCSprite::create("sog.png"_spr);

    return spr;
}