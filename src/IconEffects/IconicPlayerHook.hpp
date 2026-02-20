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
        IconicConfig* config = nullptr;

        struct DefaultValues
        {
            cocos2d::ccColor3B primary;
            cocos2d::ccColor3B secondary;
            cocos2d::ccColor3B glow;
            cocos2d::ccColor3B ghost;
            cocos2d::ccColor3B trail;
            cocos2d::ccColor3B wave;
        };
        DefaultValues values;

        void saveDefault();
        void restoreDefault();

    public:
        static IconicPlayerHook* create(PlayerObject* player);
        static IconicPlayerHook* create(SimplePlayer* simple);

        void setEnabled(bool enabled);
        bool isEnabled();

        void setGamemode(IconicGamemodeType gamemode, bool player2);

        virtual void update(float dt);
        virtual bool init();
};