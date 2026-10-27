#pragma once

#include <vector>
#include <functional>
#include <asp/fs.hpp>

class ShortcutVisualConfig;

class ShortcutPack
{
    protected:
        std::vector<ShortcutVisualConfig*> configs = {};

    public:
        void save(asp::fs::PathView path);
        void load(asp::fs::PathView path, std::function<void(std::vector<uint8_t>)> readCallback);

        void addConfig(ShortcutVisualConfig* conf);
};