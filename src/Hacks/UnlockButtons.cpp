/*#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/CCSpriteGrayscale.hpp>
#include "../Client/Client.h"

class $modify(CCSpriteGrayscale)
{
    static CCSpriteGrayscale* createWithSpriteFrame(CCSpriteFrame* spr)
    {
        if (Client::GetModuleEnabled("unlock-buttons"))
            return as<CCSpriteGrayscale*>(CCSprite::createWithSpriteFrame(spr));

        return CCSpriteGrayscale::createWithSpriteFrame(spr);
    }
};

#endif*/