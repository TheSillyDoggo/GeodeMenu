#include "Module.hpp"
#include "ModuleNode.hpp"
#include "../SafeMode/SafeMode.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../GUI/FloatingButton/ModuleShortcutButton.hpp"
#include "../GUI/FloatingButton/FloatingUIManager.hpp"
#include "../Notifications/NotificationManager.hpp"
#include "../SafeMode/Modules/DisableCheatsInMenu.hpp"
#include "../FloatingShortcuts/ShortcutManager.hpp"

using namespace geode::prelude;
using namespace qolmod;

void Module::setUserEnabled(bool enabled)
{
    this->userEnabled = enabled;

    SafeMode::get()->onModuleToggled(this);

    save();
}

bool Module::getUserEnabled()
{
    return userEnabled;
}

void Module::setForceDisabled(bool forced)
{
    forceDisabled = forced;
}

bool Module::getForceDisabled()
{
    return forceDisabled;
}

bool Module::getRealEnabled()
{
    if (forceDisabled)
        return false;

    if (getSafeModeTrigger() != SafeModeTrigger::None)
    {
        if (SafeModeDisableCheats::get()->getRealEnabled())
            return false;
    }

    if (auto pl = PlayLayer::get(); pl && pl->m_started)
    {
        if (pl->m_isPlatformer || pl->m_player1->m_isDead || (pl->m_player2 && pl->m_player2->m_isDead))
            return userEnabled;

        bool inRange;
        bool ret = enableRanges.getEnable(pl->getCurrentPercent(), userEnabled, &inRange);

        if (inRange && ret != userEnabled)
        {
            SafeMode::get()->onModuleToggled(this);
        }

        if (inRange != lastRetRange)
        {
            onRangeToggleSendNotif(ret);
            lastRetRange = inRange;
        }

        return ret;
    }
    lastRetRange = false;
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

void Module::setDisabledMessage(std::string str)
{
    this->disabledMessage = str;
}

std::string Module::getDisabledMessage()
{
    return disabledMessage;
}

void Module::genericLoad()
{
    enableRanges.load(Mod::get()->getSavedValue<matjson::Value>(fmt::format("{}_enableranges", getID()), {}));
}

void Module::genericSave()
{
    Mod::get()->setSavedValue<matjson::Value>(fmt::format("{}_enableranges", getID()), enableRanges.save());
}

bool Module::shouldSave()
{
    if (getID() == "test")
        return false;

    if (getID().empty())
        return false;

    return true;
}

void Module::save()
{
    if (!shouldSave())
        return;

    Mod::get()->setSavedValue<bool>(fmt::format("{}_enabled", getID()), getUserEnabled());
}

void Module::load()
{
    setUserEnabled(Mod::get()->getSavedValue<bool>(fmt::format("{}_enabled", getID()), defaultEnabled));
    // nothing should be favourited by default
    setFavourited(Mod::get()->getSavedValue<bool>(fmt::format("{}_favourited", getID()), false));
    loadKeyConfig();
}

ModuleNode* Module::getNode()
{
    return ModuleNode::create(this);
}

std::string Module::getName()
{
    return LocalisationManager::get()->getLocalisedString(fmt::format("names/{}", getID()));
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
    return LocalisationManager::get()->getLocalisedString(fmt::format("descriptions/{}", getID()));
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

void Module::setDisableWarning(std::string warning)
{
    this->onDisableWarning = warning;
}

void Module::setEnableWarning(std::string warning)
{
    this->onEnableWarning = warning;
}

bool Module::showDisableWarning()
{
    return onDisableWarning.empty() ? false : !Mod::get()->hasSavedValue(fmt::format("{}_disablewarningshown", getID()));
}

bool Module::showEnableWarning()
{
    return onEnableWarning.empty() ? false : !Mod::get()->hasSavedValue(fmt::format("{}_enablewarningshown", getID()));
}

std::string Module::getOnDisableWarning()
{
    return onDisableWarning;
}

std::string Module::getOnEnableWarning()
{
    return onDisableWarning;
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

void Module::addOption(Module* option)
{
    if (std::find(options.begin(), options.end(), option) != options.end())
        return;

    options.push_back(option);
    option->setParent(this);
}

std::vector<Module*>& Module::getOptions()
{
    return options;
}

void Module::setParent(Module* parent)
{
    this->parent = parent;
}

void Module::setPriority(int sortPriority)
{
    this->sortPriority = sortPriority;
}

int Module::getSortPriority()
{
    return sortPriority;
}

Module* Module::getParent()
{
    return parent;
}

Module* Module::getByID(std::string id)
{
    for (auto& mod : moduleMap)
    {
        if (mod->getID() == id)
            return mod;
    }

    return nullptr;
}

void Module::saveKeyConfig()
{
    if (!shouldSave())
        return;

    Mod::get()->setSavedValue(fmt::format("{}_keyconfig", getID()), keyConfig.save());
}

void Module::loadKeyConfig()
{
    auto value = Mod::get()->getSavedValue<matjson::Value>(fmt::format("{}_keyconfig", getID()));
    KeyConfigStruct keyconf;
    keyconf.load(value);

    setKeybind(keyconf);
}

KeyConfigStruct Module::getKeybind()
{
    return keyConfig;
}

void Module::setKeybind(KeyConfigStruct key)
{
    this->keyConfig = key;

    saveKeyConfig();
}

void Module::removeKeybind()
{
    this->keyConfig = {};

    saveKeyConfig();
}

void Module::onKeybindActivated(KeyState state)
{
    bool en = getUserEnabled();

    switch (keyConfig.type)
    {
        case KeybindType::Toggle:
            en = !getUserEnabled();

            if (!(state.isDown || state.isRepeat))
                return;
            
            break;

        case KeybindType::Hold:
            en = state.isDown;

            if (state.isRepeat)
                return;
            break;

        case KeybindType::HoldInverted:
            en = !state.isDown;

            if (state.isRepeat)
                return;
            break;
    }

    if (getUserEnabled() == en)
        return;

    this->setUserEnabled(en);

    onToggle();
    ModuleNode::updateAllNodes(nullptr);

    NotificationManager::get()->notifyToast(getNotificationString());
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

bool Module::shouldShortcutShowActivated()
{
    return getRealEnabled();
}

std::string Module::getNotificationString()
{
    auto str = getUserEnabled() ? "ui/notification-mod-enabled" : "ui/notification-mod-disabled";

    return utils::string::replace(LocalisationManager::get()->getLocalisedString(str), "%s", getName());
}

void Module::onRangeToggleSendNotif(bool en)
{
    if (en == userEnabled)
        return;

    log::error("entered range: {}, {}, {}", getID(), lastRetRange, en);
}

qolmod::Ranges* Module::getRanges()
{
    return &enableRanges;
}

#include <rapidfuzz/fuzz.hpp>

float Module::getSearchWeight(std::string query)
{
    float w = 0;

    w += (rapidfuzz::fuzz::ratio(query, getName()) / 100.0f) * 8;
    w += (rapidfuzz::fuzz::ratio(query, getID()) / 100.0f) * 3;
    w += (rapidfuzz::fuzz::ratio(query, getDescription()) / 100.0f) * 3;

    /*if (w < 4.5f)
    {
        w = 0;
    }
    else
    {*/
        if (!getParent())
            w += 0.5f;
    //}

    return w;
}