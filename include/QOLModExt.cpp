#include "QOLModExt.hpp"
#include "../src/Client/Client.h"
#include "../src/Client/Windows/Labels.hpp"

using namespace QOLModExt;

Module* moduleFromModuleExt(ModuleExt* modExt)
{
    Module* mod;

    switch (modExt->getType())
    {
        case ModuleExtType::Boolean:
            mod = new Module(modExt->getName(), modExt->getID(), modExt->getDescription(), modExt->getEnabledByDefault());
            break;
    }

    mod->onToggle = modExt->getOnToggle();

    if (modExt->getCanBeToggled())
    {
        mod->setIncompatible(modExt->getCantBeToggledWarning());
    }

    if (modExt->getEnabled())
        mod->enabled = true;

    return mod;
}

Window* windowFromWindowExt(WindowExt* wndExt)
{
    auto wnd = new Window();
    wnd->id = wndExt->getID();
    wnd->priority = wndExt->getPriority();
    wnd->name = wndExt->getName();

    for (auto mod : wndExt->getModules())
    {
        auto m = moduleFromModuleExt(mod);

        wnd->modules.push_back(m);
    }

    return wnd;
}

$execute
{
    new EventListener<EventFilter<PushWindowEvent>>(+[](PushWindowEvent* ev)
    {
        auto wnd = windowFromWindowExt(ev->window);

        Client::get()->windows.push_back(wnd);

        for (auto mod : ev->window->getModules())
        {
            delete mod;
        }

        delete ev->window;

        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<ToggleModuleEvent>>(+[](ToggleModuleEvent* ev)
    {
        auto mod = Module::get(ev->id);

        if (mod->enabled != ev->enabled)
            mod->onToggleAndroid(nullptr);

        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<CommentEmojisOverrideLabelCreatePrivateEventPleaseDoNotUseOrIWillTouchYou>>(+[](CommentEmojisOverrideLabelCreatePrivateEventPleaseDoNotUseOrIWillTouchYou* ev)
    {
        Labels::get()->createFuncCommentEmojis = ev->function;

        return ListenerResult::Stop;
    });
};

// idk bro i feel like dying im very tired i dont know what most of this does
// i should sleep
/*
$on_mod(Loaded)
{
    int i = 0;
    for (Mod* mod : Loader::get()->getAllMods())
    {
        if (mod->shouldLoad() && mod->getMetadata().getSettings().size() != 0)
        {
            auto wnd = QOLModExt::createWindow(mod->getID());
            wnd->setName(mod->getName());
            wnd->setPriority(10000 * (i + 1));

            for (auto setting : mod->getMetadata().getSettings())
            {
                if (setting.second["type"] == "bool")
                {
                    std::string id = fmt::format("{}/{}", mod->getID(), setting.first);

                    auto mod = QOLModExt::createModule(id);
                    mod->setName(setting.second["name"].asString().unwrapOr("NULL"));

                    wnd->addModule(mod);
                }
            }
            
            QOLModExt::pushWindow(wnd);

            i++;
        }
    }
};
*/