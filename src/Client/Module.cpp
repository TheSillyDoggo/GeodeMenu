#include "Module.hpp"
#include "ModuleNode.hpp"
#include "../SafeMode/SafeMode.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../GUI/FloatingButton/ModuleShortcutButton.hpp"
#include "../GUI/FloatingButton/FloatingUIManager.hpp"
#include "../Notifications/NotificationManager.hpp"
#include "../SafeMode/Modules/DisableCheatsInMenu.hpp"

using namespace geode::prelude;

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
    loadShortcutConfig();
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
    this->setUserEnabled(!getUserEnabled());

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

void Module::saveShortcutConfig()
{
    Mod::get()->setSavedValue<bool>(fmt::format("{}_shortcutenabled", getID()), shortcutEnabled);

    auto conf = matjson::Value::object();
    conf["overlay-sprite"] = shortcutConf.shortcutOverlay;
    conf["off-bg-sprite"] = shortcutConf.bgOffSprite;
    conf["on-bg-sprite"] = shortcutConf.bgOnSprite;
    conf["scale"] = shortcutConf.scale;
    conf["opacity"] = shortcutConf.opacity;
    conf["movable"] = shortcutConf.isMovable;
    conf["visibility/in-game"] = shortcutConf.visibility.showInGame;
    conf["visibility/in-pause-menu"] = shortcutConf.visibility.showInPauseMenu;
    conf["visibility/in-menu"] = shortcutConf.visibility.showInMenu;
    conf["visibility/in-editor"] = shortcutConf.visibility.showInEditor;
    conf["visibility/in-pause-editor"] = shortcutConf.visibility.showInEditorPauseMenu;
    conf["colour_colour"] = shortcutConf.colour.customColour;
    conf["colour_opacity"] = shortcutConf.colour.opacity;
    conf["colour_chromaspeed"] = shortcutConf.colour.chromaSpeed;
    conf["colour_type"] = (int)shortcutConf.colour.type;

    Mod::get()->setSavedValue<matjson::Value>(fmt::format("{}_shortcutconf", getID()), conf);
}

void Module::loadShortcutConfig()
{
    auto json = Mod::get()->getSavedValue<matjson::Value>(fmt::format("{}_shortcutconf", getID()), {});
    ModuleShortcutConfig conf;

    conf.shortcutOverlay = json["overlay-sprite"].asString().unwrapOr(conf.shortcutOverlay);
    conf.bgOffSprite = json["off-bg-sprite"].asString().unwrapOr(conf.bgOffSprite);
    conf.bgOnSprite = json["on-bg-sprite"].asString().unwrapOr(conf.bgOnSprite);
    conf.scale = json["scale"].asDouble().unwrapOr(conf.scale);
    conf.opacity = json["opacity"].asDouble().unwrapOr(conf.opacity);
    conf.isMovable = json["movable"].asBool().unwrapOr(conf.isMovable);
    conf.visibility.showInGame = json["visibility/in-game"].asBool().unwrapOr(true);
    conf.visibility.showInPauseMenu= json["visibility/in-pause-menu"].asBool().unwrapOr(true);
    conf.visibility.showInMenu = json["visibility/in-menu"].asBool().unwrapOr(true);
    conf.visibility.showInEditor = json["visibility/in-editor"].asBool().unwrapOr(true);
    conf.visibility.showInEditorPauseMenu = json["visibility/in-pause-editor"].asBool().unwrapOr(true);
    if (json["colour_colour"].isObject())
    {
        conf.colour.customColour.r = json["colour_colour"]["r"].asInt().unwrapOr(255);
        conf.colour.customColour.g = json["colour_colour"]["g"].asInt().unwrapOr(255);
        conf.colour.customColour.b = json["colour_colour"]["b"].asInt().unwrapOr(255);
    }
    conf.colour.opacity = json["colour_opacity"].asDouble().unwrapOr(1);
    conf.colour.chromaSpeed = json["colour_chromaspeed"].asDouble().unwrapOr(1);
    conf.colour.type = (ColourConfigType)json["colour_type"].asInt().unwrapOr(0);

    setShortcutConfig(Mod::get()->getSavedValue<bool>(fmt::format("{}_shortcutenabled", getID()), false), conf);
}

void Module::setShortcutConfig(bool enabled, ModuleShortcutConfig conf)
{
    if (shortcutNode)
    {
        FloatingUIManager::get()->removeButton(static_cast<ModuleShortcutButton*>(shortcutNode));
        shortcutNode = nullptr;
    }

    this->shortcutConf = conf;
    this->shortcutEnabled = enabled;

    if (shortcutEnabled)
    {
        auto btn = ModuleShortcutButton::create(this);
        btn->setBackgroundSprites(shortcutConf.bgOffSprite, shortcutConf.bgOnSprite);
        btn->setOverlaySprite(shortcutConf.shortcutOverlay);
        btn->setButtonVisibilityConfig(shortcutConf.visibility);
        btn->setMovable(shortcutConf.isMovable);
        btn->setBaseScale(shortcutConf.scale);
        btn->setBaseOpacity(shortcutConf.opacity);

        shortcutNode = btn;
    }

    saveShortcutConfig();
}

bool Module::isShortcutEnabled()
{
    return shortcutEnabled;
}

ModuleShortcutConfig Module::getShortcutConfig()
{
    return shortcutConf;
}

std::string Module::getNotificationString()
{
    auto str = getUserEnabled() ? "ui/notification-mod-enabled" : "ui/notification-mod-disabled";

    return utils::string::replace(LocalisationManager::get()->getLocalisedString(str), "%s", getName());
}