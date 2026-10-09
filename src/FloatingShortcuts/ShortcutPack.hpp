#pragma once

#include <vector>
#include <filesystem>
#include <functional>

class ShortcutVisualConfig;

class ShortcutPack
{
    protected:
        std::vector<ShortcutVisualConfig*> configs = {};

    public:
        void save(std::filesystem::path path);
        void load(std::filesystem::path path, std::function<void(std::vector<uint8_t>)> readCallback);

        void addConfig(ShortcutVisualConfig* conf);
};