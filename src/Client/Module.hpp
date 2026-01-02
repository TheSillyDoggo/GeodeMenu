#pragma once

#include <Geode/Geode.hpp>
#include "HookMacro.hpp"
#include "../Utils/DLL.hpp"
#include "../Keybinds/KeyConfigStruct.hpp"
#include "../Keybinds/KeyState.hpp"
#include "ModuleShortcutConfig.hpp"

#define SUBMIT_HACK(func) \
$on_mod(Loaded) \
{ \
    func::get(); \
}

#define SUBMIT_OPTION(func, option) \
$on_mod(Loaded) \
{ \
    func::get()->addOption(option::get()); \
}

#define MODULE_GETTER(func) \
static func* get() \
{ \
    static func* instance = nullptr; \
    if (!instance) \
    { \
        instance = new func(); \
        instance->load(); \
        Module::moduleMap.push_back(instance); \
    } \
    return instance; \
}

#define MODULE_SETUP(func) \
MODULE_GETTER(func) \
func()

class ModuleNode;

enum class SafeModeTrigger
{
    None,
    Attempt,
    LevelLoad,
    Custom,
};

class Module
{
    protected:
        friend class AndroidUI;

        static inline std::vector<Module*> moduleMap = {};

        std::string name = "";
        std::string id = "";
        std::string category = "";
        std::string description = "";
        bool defaultEnabled = false;
        bool disabled = false;
        bool favourited = false;
        SafeModeTrigger trigger = SafeModeTrigger::None;
        std::function<bool()> safeModeCustomTrigger = nullptr;
        Module* parent = nullptr;
        std::vector<Module*> options = {};
        std::string disabledMessage = "";
        std::string onDisableWarning = "";
        std::string onEnableWarning = "";
        int sortPriority = 8008135;
        KeyConfigStruct keyConfig = {};

        bool shortcutEnabled = false;
        cocos2d::CCNode* shortcutNode = nullptr;
        ModuleShortcutConfig shortcutConf = {};

        void DLL setName(std::string str);
        void DLL setID(std::string str);
        void DLL setCategory(std::string str);
        void DLL setDescription(std::string str);
        void DLL setDefaultEnabled(bool def);
        void DLL setDisabled(bool value);
        void DLL setSafeModeTrigger(SafeModeTrigger trigger);
        void DLL setSafeModeCustom(std::function<bool()> func);
        void DLL setParent(Module* parent);
        void DLL setDisabledMessage(std::string str);
        void DLL setDisableWarning(std::string warning);
        void DLL setEnableWarning(std::string warning);
        void DLL setPriority(int sortPriority);

        bool shouldSave();

        virtual void save();
        virtual void load();

        void saveKeyConfig();
        void loadKeyConfig();

        void saveShortcutConfig();
        void loadShortcutConfig();

    private:
        bool userEnabled = false;
        bool forceDisabled = false;

    public:
        static Module* getByID(std::string id);
        static std::vector<Module*> getAllFavourited();
        static std::vector<Module*>& getAll();
        static void sortAlphabetically(std::vector<Module*>* map);

        virtual void setUserEnabled(bool enabled);
        virtual bool getUserEnabled();

        void setFavourited(bool favourited);
        bool isFavourited();

        void setForceDisabled(bool forced);
        bool getForceDisabled();

        bool getRealEnabled();

        void addOption(Module* option);
        std::vector<Module*>& getOptions();

        void setKeybind(KeyConfigStruct key);
        void removeKeybind();

        KeyConfigStruct getKeybind();

        void setShortcutConfig(bool enabled, ModuleShortcutConfig conf);
        bool isShortcutEnabled();
        ModuleShortcutConfig getShortcutConfig();

        Module* getParent();
        std::string getName();
        std::string getID();
        std::string getCategory();
        std::string getDescription();
        std::string getDisabledMessage();
        std::string getOnDisableWarning();
        std::string getOnEnableWarning();
        bool showDisableWarning();
        bool showEnableWarning();
        SafeModeTrigger getSafeModeTrigger();
        std::function<bool()> getSafeModeCustom();
        int getSortPriority();
        bool isDisabled();

        virtual ModuleNode* getNode();
        virtual void DLL onToggle();
        virtual void onKeybindActivated(KeyState state);
        virtual bool shouldShortcutShowActivated();
};