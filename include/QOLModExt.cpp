#include "QOLModExt.hpp"
#include "../src/Client/Client.h"

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
};