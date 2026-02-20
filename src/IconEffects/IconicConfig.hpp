#pragma once

#include <Geode/Geode.hpp>
#include "ColourConfig.hpp"
#include "IconicCommon.hpp"

class IconicConfig
{
    protected:
        std::string saveStr = "";
        ColourConfig primary = {};
        ColourConfig secondary = {};
        ColourConfig glow = {};

    public:
        static IconicConfig* create(IconicGamemodeType gamemode, bool player2);

        cocos2d::ccColor3B getPrimary();
        cocos2d::ccColor3B getSecondary();
        cocos2d::ccColor3B getGlow();

        ColourConfig getPrimaryConfig();
        ColourConfig getSecondaryConfig();
        ColourConfig getGlowConfig();

        void setPrimaryConfig(ColourConfig config);
        void setSecondaryConfig(ColourConfig config);
        void setGlowConfig(ColourConfig config);

        void save();
        void load();
};