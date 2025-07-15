#pragma once

#include <Geode/Geode.hpp>
#include "HookMacro.hpp"

#define SUBMIT_HACK(func) \
$execute \
{ \
    func::get(); \
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

        void setName(std::string str);
        void setID(std::string str);
        void setCategory(std::string str);
        void setDescription(std::string str);
        void setDefaultEnabled(bool def);
        void setDisabled(bool value);

        void save();
        void load();

    private:
        bool userEnabled = false;
        bool forceDisabled = false;

        std::vector<geode::Hook*> hooks = {};

        void disableHooks();
        void enableHooks();

    public:
        static Module* getByID(std::string id);
        static std::vector<Module*> getAllFavourited();
        static void sortAlphabetically();

        void setUserEnabled(bool enabled);
        bool getUserEnabled();

        void setFavourited(bool favourited);
        bool isFavourited();

        void setForceDisabled(bool forced);
        bool getForceDisabled();

        bool getRealEnabled();

        void addHook(geode::Hook* hook);

        std::string getName();
        std::string getID();
        std::string getCategory();
        std::string getDescription();
        bool isDisabled();

        virtual ModuleNode* getNode();
};