#pragma once

#include <Geode/Geode.hpp>
#include "ColourConfig.hpp"
#include "IconicCommon.hpp"

class IconicConfig
{
    protected:
        std::string saveStr = "";
        IconicGamemodeType gamemode;
        bool player2;
        
        ColourConfig primary = {};
        ColourConfig secondary = {};
        ColourConfig glow = {};
        ColourConfig trail = {};
        ColourConfig ghost = {};
        ColourConfig waveTrail = {};
        bool primaryEnabled = false;
        bool secondaryEnabled = false;
        bool glowEnabled = false;
        bool trailEnabled = false;
        bool ghostEnabled = false;
        bool waveTrailEnabled = false;

    public:
        static IconicConfig* create(IconicGamemodeType gamemode, bool player2);

        cocos2d::ccColor3B getDefault(IconicEffectType type);

        cocos2d::ccColor3B getPrimary(bool ignoreP2 = false);
        cocos2d::ccColor3B getSecondary(bool ignoreP2 = false);
        cocos2d::ccColor3B getGlow();
        cocos2d::ccColor3B getTrail();
        cocos2d::ccColor3B getGhost();
        cocos2d::ccColor3B getWaveTrail();

        ColourConfig getPrimaryConfig();
        ColourConfig getSecondaryConfig();
        ColourConfig getGlowConfig();
        ColourConfig getTrailConfig();
        ColourConfig getGhostConfig();
        ColourConfig getWaveTrailConfig();

        void setPrimaryConfig(ColourConfig config);
        void setSecondaryConfig(ColourConfig config);
        void setGlowConfig(ColourConfig config);
        void setTrailConfig(ColourConfig config);
        void setGhostConfig(ColourConfig config);
        void setWaveTrailConfig(ColourConfig config);

        bool getUseOverride(IconicEffectType type);
        void setUseOverride(IconicEffectType type, bool v);

        void save();
        void load();
};