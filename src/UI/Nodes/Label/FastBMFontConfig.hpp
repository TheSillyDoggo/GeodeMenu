#ifdef QOLMOD_FAST_CONFIG

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCBMFontConfiguration.hpp>

class FastBMFontConfig : public cocos2d::CCBMFontConfiguration
{
    public:
        static FastBMFontConfig* create(const char* fntFile);
        static void quickLoad(const char* fntFile);

        gd::set<unsigned int>* parseConfigFile(const char* fntFile);

        bool init(const char* fntFile);
};

class $modify (FastBMFontConfiguration, cocos2d::CCBMFontConfiguration)
{
    static CCBMFontConfiguration* create(const char *FNTfile);
};

#endif