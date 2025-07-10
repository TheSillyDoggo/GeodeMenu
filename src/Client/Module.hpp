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
        Module::moduleMap.emplace(instance->id, instance); \
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

        static inline std::unordered_map<std::string, Module*> moduleMap = {};

        std::string name = "";
        std::string id = "";
        std::string category = "";

        void setName(std::string str);
        void setID(std::string str);
        void setCategory(std::string str);

    private:

        bool userEnabled = false;
        bool forceDisabled = false;

        std::vector<geode::Hook*> hooks = {};

        void updateHooks();

    public:
        static Module* getByID(std::string id);

        void setUserEnabled(bool enabled);
        bool getUserEnabled();

        void setForceDisabled(bool forced);
        bool getForceDisabled();

        bool getRealEnabled();

        void addHook(geode::Hook* hook);

        std::string getCategory();

        virtual ModuleNode* getNode();
};