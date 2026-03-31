#pragma once

namespace cocos2d
{
    class CCSprite;
};

namespace qolmod
{
    enum class MenuAnimation
    {
        None = 0,
        FromTop = 1,
        FromBottom = 2,
        FromLeft = 3,
        FromRight = 4,
        Scale = 5,
        FadeIn = 6
    };

    enum class BackgroundType
    {
        GDBrown = 1,
        GDBlue = 2,
        GDGreen = 3,
        GDPurple = 4,
        GDGrey = 5,
        Gradient = -1,
        Darken = -2,
        Geode01 = -4,
        Geode02 = -5,
        Geode03 = -6,
        Custom = -7,
    };

    class ThemeManager
    {
        protected:
            MenuAnimation anim = MenuAnimation::Scale;
            BackgroundType background = BackgroundType::Geode03;
            std::error_code moveErrorCode;
            std::error_code setErrorCode;

            void save();
            void load();
        
        public:
            static ThemeManager* get();

            MenuAnimation getAnimation();
            void setAnimation(MenuAnimation animation);

            BackgroundType getBackground();
            void setBackground(BackgroundType background);

            void setCustomSprite(std::filesystem::path path);
            cocos2d::CCSprite* createCustomSprite(bool returnDefault = true);
    };
};