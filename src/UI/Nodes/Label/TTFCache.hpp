#pragma once

#include <Geode/Geode.hpp>
#include "FallbackLabelProtocol.hpp"

class TTFCache
{
    protected:
        std::unordered_map<std::string, cocos2d::CCTexture2D*> stringsCached = {};

        void addTexture(std::string text);

    public:
        static TTFCache* get();

        void flushTextures();
        void preloadTextures();

        cocos2d::CCTexture2D* getTexture(std::string text);
};