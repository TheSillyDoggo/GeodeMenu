#include "ShortcutManager.hpp"
#include <Geode/Geode.hpp>
#include "Module.hpp"
#include <FloatingButton/ModuleShortcutButton.hpp>
#include <FloatingButton/FloatingUIManager.hpp>
#include "ShortcutPack.hpp"

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
    auto path = Mod::get()->getSaveDir() / "shortcuts.shpak";

    if (std::filesystem::exists(path))
    {
        ShortcutPack pack;
        pack.load(path, [this](std::vector<uint8_t> data){
            ShortcutVisualConfig conf;
            conf.loadV2(data);

            auto mod = Module::getByID(conf.getModuleID());
            if (mod)
                configs[mod] = conf;
        });
    }

    bool needsSave = false;
    for (auto& mod : Module::getAll())
    {
        if (!configs.contains(mod))
        {
            if (Mod::get()->hasSavedValue(fmt::format("{}_shortcutconf", mod->getID())))
            {
                configs[mod].loadV1(
                    Mod::get()->getSavedValue<matjson::Value>(fmt::format("{}_shortcutconf", mod->getID()), {}),
                    Mod::get()->getSavedValue<bool>(fmt::format("{}_shortcutenabled", mod->getID()), false),
                    mod->getID()
                );

                needsSave = true;
            }
        }
    }

    if (needsSave)
        saveAll();
    
    for (auto& conf : configs)
    {
        if (conf.second.isEnabled())
        {
            if (nodes.contains(conf.first))
                nodes[conf.first]->updateSettings();
            else
                nodes[conf.first] = ModuleShortcutButton::create(conf.first);
        }
    }
}

void ShortcutManager::saveAll()
{
    ShortcutPack pack;

    for (auto& conf : configs)
    {
        pack.addConfig(&conf.second);
    }

    pack.save(Mod::get()->getSaveDir() / "shortcuts.shpak");
}

ShortcutVisualConfig* ShortcutManager::getConfig(Module* module)
{
    return &(configs[module]);
}

void ShortcutManager::updateConfig(Module* module)
{
    saveAll();

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