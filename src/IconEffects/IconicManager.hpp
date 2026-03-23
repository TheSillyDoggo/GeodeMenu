#pragma once

#include "IconicPlayerHook.hpp"
#include "IconicConfig.hpp"

namespace geode
{
    class Mod;
};

class IconicManager
{
    protected:
        std::unordered_map<IconicGamemodeType, std::pair<IconicConfig*, IconicConfig*>> configs = {};
        bool seperate = true;
        IconicDualMode dualMode = IconicDualMode::Invert;
        std::vector<geode::Mod*> incompatibleMods = {};
        geode::Mod* fineOutline = nullptr;

        void setup();

    public:
        static IconicManager* get();

        IconicConfig* getConfig(IconicGamemodeType gamemode, bool player2);

        bool getSeperateColours();
        void setSeperateColours(bool v);

        bool areIncompatibleModsLoaded();
        std::vector<geode::Mod*> getIncompatibleMods();
        void handleIncompatibility(std::string id);

        bool isFineOutlineLoaded();
        cocos2d::ccColor3B getFineOutineColour();

        IconicDualMode getDualMode();
        void setDualMode(IconicDualMode mode);
};