#pragma once

#include <Geode/Geode.hpp>
#include "IconicCommon.hpp"
#include "IconicManager.hpp"
#include "IconicConfig.hpp"

class IconicPlayerHook : public cocos2d::CCNode
{
    protected:
        PlayerObject* player = nullptr;
        SimplePlayer* simple = nullptr;
        bool enabled = false;
        bool player2 = false;
        IconicConfig* config = nullptr;
        std::vector<geode::WeakRef<cocos2d::CCSprite>> fireDashes = {};
        std::vector<geode::WeakRef<cocos2d::CCSprite>> spiderTeleports = {};

    public:
        static IconicPlayerHook* create(PlayerObject* player);
        static IconicPlayerHook* create(SimplePlayer* simple);

        void setEnabled(bool enabled);
        bool isEnabled();

        void setGamemode(IconicGamemodeType gamemode, bool player2);

        void onBeginDash(cocos2d::CCSprite* sprite);
        void onSpiderTeleport(cocos2d::CCSprite* sprite);

        virtual void update(float dt);
        virtual bool init();
};