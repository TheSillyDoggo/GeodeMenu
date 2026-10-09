#pragma once

#include "ModuleShortcutConfig.hpp"
#include "ShortcutVisualConfig.hpp"

class Module;
class ModuleShortcutButton;
class ShortcutGroupButton;

namespace qolmod
{
    class ShortcutManager
    {
        protected:
            std::unordered_map<Module*, ShortcutVisualConfig> configs = {};
            std::unordered_map<Module*, ModuleShortcutButton*> nodes = {};
            std::unordered_map<int, std::vector<ModuleShortcutButton*>> groups = {};
            std::unordered_map<int, ShortcutGroupButton*> groupButtons = {};

        public:
            static ShortcutManager* get();

            void saveAll();
            void loadAll();

            ShortcutVisualConfig* getConfig(Module* module);

            void updateGroup(Module* module);
            void updateConfig(Module* module);
    };
};