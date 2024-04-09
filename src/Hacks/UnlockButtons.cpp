/*#include <Geode/Geode.hpp>
#include <Geode/modify/CCSpriteGrayscale.hpp>
#include "../Client/Client.h"

class $modify(CCSpriteGrayscale)
{
    static CCSpriteGrayscale* createWithSpriteFrameName(char const* p0)
    {
        //if (Client::GetModuleEnabled("unlock-buttons"))
        log::info("name: {}", p0);
        //CCScene::get()->addChild(as<CCSpriteGrayscale*>(CCSprite::createWithSpriteFrameName(p0.c_str())));

        return CCSpriteGrayscale::createWithSpriteFrameName(p0);
    }
};*/