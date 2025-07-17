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

    save();
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

// TODO: Fix hooks

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
    for (auto hook : hooks)
    {
        (void)hook->enable();
    }
}

void Module::disableHooks()
{
    for (auto hook : hooks)
    {
        (void)hook->disable();
    }
}

void Module::save()
{
    Mod::get()->setSavedValue<bool>(fmt::format("{}_enabled", getID()), getUserEnabled());
}

void Module::load()
{
    setUserEnabled(Mod::get()->getSavedValue<bool>(fmt::format("{}_enabled", getID()), defaultEnabled));
    // nothing should be favourited by default
    setFavourited(Mod::get()->getSavedValue<bool>(fmt::format("{}_favourited", getID()), false));
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

void Module::setDefaultEnabled(bool def)
{
    this->defaultEnabled = def;
}

void Module::setDisabled(bool value)
{
    this->disabled = value;
}

bool Module::isDisabled()
{
    return disabled;
}

void Module::setFavourited(bool favourited)
{
    this->favourited = favourited;

    Mod::get()->setSavedValue<bool>(fmt::format("{}_favourited", getID()), favourited);
}

bool Module::isFavourited()
{
    return favourited;
}

void Module::setSafeModeTrigger(SafeModeTrigger trigger)
{
    this->trigger = trigger;
}

SafeModeTrigger Module::getSafeModeTrigger()
{
    return this->trigger;
}

void Module::setSafeModeCustom(std::function<bool()> func)
{
    this->safeModeCustomTrigger = func;
}

std::function<bool()> Module::getSafeModeCustom()
{
    return safeModeCustomTrigger;
}

void Module::onToggle()
{

}

Module* Module::getByID(std::string id)
{
    log::info("todo: implement");
    return nullptr;
    //if (moduleMap.contains(id))
        //return moduleMap[id];

    // will probably crash but i dont fucking care anymore
    //return nullptr;
}

std::vector<Module*> Module::getAllFavourited()
{
    std::vector<Module*> favourites = {};

    for (auto mod : moduleMap)
    {
        if (mod->isFavourited())
            favourites.push_back(mod);
    }

    return favourites;
}

std::vector<Module*>& Module::getAll()
{
    return moduleMap;
}