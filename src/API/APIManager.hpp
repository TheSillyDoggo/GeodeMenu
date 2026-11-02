#pragma once

#include <vector>

namespace geode
{
    class Mod;
};

namespace qolmod
{
    class APIManager
    {
        protected:
            std::vector<geode::Mod*> anticheatMods = {};
            bool enabledAntiCheat = false;

        public:
            static APIManager* get();

            void addAntiCheatMod(geode::Mod* mod);
            void removeAntiCheatMod(geode::Mod* mod);
            const std::vector<geode::Mod*> getAntiCheatMods();
            const bool isAntiCheatEnabled();
    };
};