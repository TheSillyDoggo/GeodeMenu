#include "ShortcutManager.hpp"
#include <Geode/Geode.hpp>
#include "Module.hpp"
#include <FloatingButton/ModuleShortcutButton.hpp>
#include <FloatingButton/FloatingUIManager.hpp>
#include "ShortcutPack.hpp"
#include <asp/time.hpp>

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

    auto t1 = asp::time::Instant::now();

    if (path.exists())
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

    auto t2 = asp::time::Instant::now();

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

    auto t3 = asp::time::Instant::now();

    if (needsSave)
        saveAll();

    auto t4 = asp::time::Instant::now();
    
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

    log::debug("ShortcutManager::loadAll() - pack: {}, legacy: {}, resave: {}, create: {}", t1.absDiff(t2).toHumanString(), t2.absDiff(t3).toHumanString(), t3.absDiff(t4).toHumanString(), t4.absDiff(asp::time::Instant::now()).toHumanString());
}

void ShortcutManager::saveAll()
{
    auto t1 = asp::time::Instant::now();

    ShortcutPack pack;

    for (auto& conf : configs)
    {
        pack.addConfig(&conf.second);
    }

    auto t2 = asp::time::Instant::now();
    pack.save(Mod::get()->getSaveDir() / "shortcuts.shpak");
    log::debug("ShortcutManager::saveAll() - gen: {} - write: {}", t1.absDiff(t2).toHumanString(), t2.absDiff(asp::time::Instant::now()).toHumanString());
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
}