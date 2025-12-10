#pragma once

#include "Module.hpp"
#include "../Utils/ColourConfig.hpp"

class ColourModule : public Module
{
    protected:
        ColourConfig defaultConf;
        ColourConfig userConf;

        virtual void save();
        virtual void load();

        void setDefaultConfig(ColourConfig config);

    public:
        void setConfig(ColourConfig config);
        ColourConfig getConfig();

        ColourConfig getDefaultConfig();

        cocos2d::ccColor3B getColour();
        
        virtual ModuleNode* getNode();
};