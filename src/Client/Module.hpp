#pragma once

#include <Geode/Geode.hpp>
#include "HookMacro.hpp"
#include "../Utils/DLL.hpp"

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

        bool shouldSave();

        virtual void save();
        virtual void load();

    private:
        bool userEnabled = false;
        bool forceDisabled = false;

        std::vector<geode::Hook*> hooks = {};

        void disableHooks();
        void enableHooks();

    public:
        static Module* getByID(std::string id);
        static std::vector<Module*> getAllFavourited();
        static std::vector<Module*>& getAll();
        static void sortAlphabetically();

        virtual void setUserEnabled(bool enabled);
        virtual bool getUserEnabled();

        void setFavourited(bool favourited);
        bool isFavourited();

        void setForceDisabled(bool forced);
        bool getForceDisabled();

        bool getRealEnabled();

        void addHook(geode::Hook* hook);

        void addOption(Module* option);
        std::vector<Module*> getOptions();

        Module* getParent();
        std::string getName();
        std::string getID();
        std::string getCategory();
        std::string getDescription();
        std::string getDisabledMessage();
        SafeModeTrigger getSafeModeTrigger();
        std::function<bool()> getSafeModeCustom();
        bool isDisabled();

        virtual ModuleNode* getNode();
        virtual void DLL onToggle();
};