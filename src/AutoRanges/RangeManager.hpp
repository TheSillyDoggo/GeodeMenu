#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Ranges.hpp"

class Module;

namespace qolmod
{
    struct ModRangeConfig
    {
        qolmod::Ranges m_ranges;
        bool m_enabled = true;
        bool m_notify = true;
    };

    enum class ModRangeMode : uint8_t
    {
        Global = 0,
        PerLevel = 1,
        Disabled = 2,
    };

    class RangeManager
    {
        protected:
            std::unordered_map<std::string, ModRangeConfig> m_configs;
            std::unordered_map<Module*, ModRangeMode> m_modes;
            bool m_needsSave = false;

            void clearEmpty();

        public:
            static RangeManager* get();

            void save();
            void load();

            const ModRangeMode& getMode(Module* module);
            void setMode(Module* module, const ModRangeMode& mode);

            ModRangeConfig* getConfig(const std::string& key);
            std::string getKey(Module* module, GJGameLevel* level);
    };
};