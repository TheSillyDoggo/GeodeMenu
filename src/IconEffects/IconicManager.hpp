#pragma once

#include "IconicPlayerHook.hpp"
#include "IconicConfig.hpp"

class IconicManager
{
    protected:
        std::unordered_map<IconicGamemodeType, std::pair<IconicConfig*, IconicConfig*>> configs = {};
        bool seperate = true;
        IconicDualMode dualMode = IconicDualMode::Invert;

        void setup();

    public:
        static IconicManager* get();

        IconicConfig* getConfig(IconicGamemodeType gamemode, bool player2);

        bool getSeperateColours();
        void setSeperateColours(bool v);

        IconicDualMode getDualMode();
        void setDualMode(IconicDualMode mode);
};