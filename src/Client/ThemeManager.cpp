#include "ThemeManager.hpp"
#include <Geode/Geode.hpp>

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
    background = (BackgroundType)Mod::get()->getSavedValue<int>("theme", (int)background);

    if (!std::filesystem::exists(Mod::get()->getConfigDir() / "ui-background.png"))
    {
        auto path = Mod::get()->getSavedValue<std::string>("custom-background-path", "");

        if (!path.empty())
        {
            std::filesystem::copy_file(
                Mod::get()->getSavedValue<std::string>("custom-background-path", ""),
                Mod::get()->getConfigDir() / "ui-background.png",
                std::filesystem::copy_options::overwrite_existing,
                moveErrorCode
            );

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

void ThemeManager::setCustomSprite(std::filesystem::path path)
{
    CCTextureCache::get()->removeTextureForKey(geode::utils::string::pathToString(Mod::get()->getConfigDir() / "ui-background.png").c_str());
    if (path.empty() || !std::filesystem::exists(path))
    {
        std::filesystem::remove(
            Mod::get()->getConfigDir() / "ui-background.png",
            setErrorCode
        );
        return;
    }

    std::filesystem::copy_file(
        path,
        Mod::get()->getConfigDir() / "ui-background.png",
        std::filesystem::copy_options::overwrite_existing,
        setErrorCode
    );
}

CCSprite* ThemeManager::createCustomSprite(bool returnDefault)
{
    auto spr = CCSprite::create(geode::utils::string::pathToString(Mod::get()->getConfigDir() / "ui-background.png").c_str());

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