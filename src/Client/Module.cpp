#include "Module.hpp"
#include "ModuleNode.hpp"

using namespace geode::prelude;

void Module::setUserEnabled(bool enabled)
{
    this->userEnabled = enabled;

    updateHooks();
}

bool Module::getUserEnabled()
{
    return userEnabled;
}

void Module::setForceDisabled(bool forced)
{
    forceDisabled = forced;

    updateHooks();
}

bool Module::getForceDisabled()
{
    return forceDisabled;
}

bool Module::getRealEnabled()
{
    if (forceDisabled)
        return false;

    return userEnabled;
}

void Module::setName(std::string str)
{
    this->name = str;
}

void Module::setID(std::string str)
{
    this->id = str;
}

void Module::setCategory(std::string str)
{
    this->category = str;
}

void Module::addHook(geode::Hook* hook)
{
    hook->setAutoEnable(false);
    (void)hook->disable();

    if (getRealEnabled())
        (void)hook->enable();

    hooks.push_back(hook);
}

void Module::updateHooks()
{
    bool enabled = getRealEnabled();

    for (auto hook : hooks)
    {
        (void)hook->disable();

        if (enabled)
            (void)hook->enable();
    }
}

ModuleNode* Module::getNode()
{
    return ModuleNode::create(this);
}

std::string Module::getName()
{
    return name;
}

std::string Module::getID()
{
    return id;
}

std::string Module::getCategory()
{
    return category;
}

Module* Module::getByID(std::string id)
{
    if (moduleMap.contains(id))
        return moduleMap[id];

    // will probably crash but i dont fucking care anymore
    return nullptr;
}