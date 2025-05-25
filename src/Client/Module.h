#pragma once

#include "ColourUtility.h"
#include <Geode/ui/TextInput.hpp>
#include "../UI/UIComponent.hpp"
#include "../Keybinds/KeyStruct.hpp"
#include <imgui-cocos.hpp>

using namespace geode::prelude;

class ModuleChangeDelegate
{
    public:
        // cant
        UIComponent* _module;

        virtual void onModuleChanged(bool enabled)
        {

        }

        virtual void initOptionsLayer(CCLayer* options)
        {

        }
};

class Module : public UIComponent
{
    protected:
        std::vector<geode::Hook*> hooks = {};
        std::vector<geode::Patch*> patches = {};

        static inline std::map<std::string, Module*> modules = {};
    public:
        static inline std::string descMod = "";

        ImVec2 lastRenderedPosition;

        std::vector<Module*> options = {};

        std::string name;
        std::string id;
        std::string description;
        std::string alert;
        std::string inCompAlert;
        bool enabled;
        bool useAlert;
        bool onceAlert;
        bool isInComp;

        KeyStruct keybind;

        bool vAlert;

        bool def;
        float value = 1.0f;

        bool mouseHeldDown = false;

        std::function<void(bool)> onToggle;
        ModuleChangeDelegate* delegate = nullptr;

        CCSize optionSizeForce = CCSizeZero;

        void addHookRaw(Result<Hook*> hook);

        void addHook(Hook* hook);
        void addPatch(Patch* patch);

        void disableHooks();
        void enableHooks();

        void disablePatches();
        void enablePatches();

        virtual void drawImGui();

        Module()
        {
            // dont add to cache
        }

        Module(std::string n, std::string i)
        {
            name = n;
            id = i;
            enabled = false;

            this->load();

            addToCache();
        }

        Module(std::string n, std::string i, std::string d, bool _def = false, std::string a = "", bool as = false)
        {
            name = n;
            id = i;
            description = d;
            enabled = _def;
            def = _def;
            useAlert = as;
            alert = a;

            this->load();

            addToCache();
        }

        static Module* get(std::string id);
        void addToCache();


        // options
        CCSize sizeForOptionsPage();

        virtual void onChange()
        {
            if (delegate)
                delegate->onModuleChanged(this->enabled);
        }

        virtual void save()
        {
            geode::prelude::Mod::get()->setSavedValue<bool>(id + "_enabled", enabled);
            keybind.saveToModule(id);
        }

        virtual void load()
        {
            keybind = KeyStruct::loadFromModule(id);
            enabled = geode::prelude::Mod::get()->getSavedValue<bool>(id + "_enabled", def);
            save();
        }

        void onInfoAndroid(CCObject* sender);
        void onOptionsAndroid(CCObject* sender);
        void onToggleAndroid(CCObject* sender);

        void setIncompatible(std::string str);
        
        virtual void makeAndroid(CCNode* menu, CCPoint pos);
};

class InfoModule : public Module
{
    public:

        InfoModule(std::string n, std::string d)
        {
            name = n;
            description = d;
        }

        void save()
        {
        }
        void load()
        {
        }
};