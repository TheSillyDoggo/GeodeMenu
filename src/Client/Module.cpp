#include "Module.hpp"
#include "ModuleNode.hpp"

using namespace geode::prelude;

void Module::setUserEnabled(bool enabled)
{
    this->userEnabled = enabled;

    if (forceDisabled)
    {
        disableHooks();
    }
    else
    {
        if (userEnabled)
            enableHooks();
        else
            disableHooks();
    }
}

bool Module::getUserEnabled()
{
    return userEnabled;
}

void Module::setForceDisabled(bool forced)
{
    forceDisabled = forced;

    if (forceDisabled)
    {
        disableHooks();
    }
    else
    {
        if (userEnabled)
            enableHooks();
        else
            disableHooks();
    }
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
    hooks.push_back(hook);

    hook->setAutoEnable(false);

    //hook->enable();

    //if (getRealEnabled())
        //(void)hook->enable();
}

void Module::enableHooks()
{
    log::error("enable");

    bool enabled = getRealEnabled();

    for (auto hook : hooks)
    {
        (void)hook->enable();
    }
}

void Module::disableHooks()
{
    log::error("disable");

    bool enabled = getRealEnabled();

    for (auto hook : hooks)
    {
        (void)hook->disable();
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

std::string Module::getDescription()
{
    return description;
}

void Module::setDescription(std::string str)
{
    this->description = str;
}

Module* Module::getByID(std::string id)
{
    if (moduleMap.contains(id))
        return moduleMap[id];

    // will probably crash but i dont fucking care anymore
    return nullptr;
}