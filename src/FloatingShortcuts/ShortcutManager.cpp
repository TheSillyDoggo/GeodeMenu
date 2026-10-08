#include "ShortcutManager.hpp"
#include <Geode/Geode.hpp>
#include "Module.hpp"
#include <FloatingButton/ModuleShortcutButton.hpp>
#include <FloatingButton/FloatingUIManager.hpp>

using namespace geode::prelude;
using namespace qolmod;

$on_game(Loaded)
{
    ShortcutManager::get()->loadAll();
}

ShortcutManager* ShortcutManager::get()
{
    static ShortcutManager instance;
    return &instance;
}

void ShortcutManager::loadAll()
{
    for (auto& mod : Module::getAll())
    {
        loadConfig(mod);
    }
}

void ShortcutManager::loadConfig(Module* module)
{
    auto dir = Mod::get()->getSaveDir() / "shortcuts";
    auto path = dir / fmt::format("{}.qolmodsh", utils::string::replace(module->getID(), "/", "=="));

    if (!std::filesystem::exists(dir))
        std::filesystem::create_directory(dir);

    if (!std::filesystem::exists(path))
    {
        configs[module].loadV1(
            Mod::get()->getSavedValue<matjson::Value>(fmt::format("{}_shortcutconf", module->getID()), {}),
            Mod::get()->getSavedValue<bool>(fmt::format("{}_shortcutenabled", module->getID()), false),
            module->getID()
        );
        configs[module].saveV2(path);
    }
    else
        configs[module].loadV2(path);

    if (configs[module].isEnabled())
    {
        if (nodes.contains(module))
            nodes[module]->updateSettings();
        else
            nodes[module] = ModuleShortcutButton::create(module);
    }
}

void ShortcutManager::saveConfig(Module* module)
{
    auto dir = Mod::get()->getSaveDir() / "shortcuts";
    auto path = dir / fmt::format("{}.qolmodsh", utils::string::replace(module->getID(), "/", "=="));

    if (!std::filesystem::exists(dir))
        std::filesystem::create_directory(dir);

    configs[module].saveV2(path);
}

ShortcutVisualConfig* ShortcutManager::getConfig(Module* module)
{
    return &(configs[module]);
}

void ShortcutManager::updateConfig(Module* module)
{
    saveConfig(module);

    if (nodes.contains(module))
        nodes[module]->updateSettings();
}

void ShortcutManager::updateGroup(Module* module)
{
    for (auto& group : groups)
    {
        std::erase(group.second, nodes[module]);
    }
    
    // auto id = configs[module].group;
    // groups[id].push_back(nodes[module]);

    // FloatingUIManager::removeButton(nodes[module]);
}