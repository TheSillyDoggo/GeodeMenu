#pragma once

#include "IconicPlayerHook.hpp"
#include "IconicConfig.hpp"

class IconicManager
{
    protected:
        std::unordered_map<IconicGamemodeType, std::pair<IconicConfig*, IconicConfig*>> configs = {};

    public:
        static IconicManager* get();

        IconicConfig* getConfig(IconicGamemodeType gamemode, bool player2);
};