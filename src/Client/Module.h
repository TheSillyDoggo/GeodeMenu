#pragma once

#include "ColourUtility.h"
#include "DrawUtils.h"

#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class ModuleChangeDelegate
{
    public:
        virtual void onModuleChanged(bool enabled)
        {

        }

        virtual void initOptionsLayer(CCLayer* options)
        {

        }
};

class Module
{
    protected:
        std::vector<geode::Hook*> hooks = {};
        std::vector<geode::Patch*> patches = {};
    public:
        static inline std::string descMod = "";

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

        bool vAlert;

        bool def;
        float value = 1.0f;

        bool mouseHeldDown = false;

        ModuleChangeDelegate* delegate = nullptr;

        void addHookRaw(Result<Hook*> hook);

        void addHook(Hook* hook);
        void addPatch(Patch* hook);

        void disableHooks();
        void enableHooks();

        void disablePatches();
        void enablePatches();

        Module()
        {

        }

        Module(std::string n, std::string i)
        {
            name = n;
            id = i;
            enabled = false;

            this->load();
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
        }

        /// @brief 
        /// @param point the position of the touch relative to where the module should be drawn
        /// @param touch touch
        /// @return should stop input passing to gd
        bool touchBegan(CCPoint point, CCTouch* touch);
        bool touchMoved(CCPoint point, CCTouch* touch);
        bool touchEndedOrCancelled(CCPoint point, CCTouch* touch, bool cancelled);

        void drawModule(CCPoint pointTopLeft);

        virtual void onChange()
        {
            if (delegate)
                delegate->onModuleChanged(this->enabled);
        }

        virtual void save()
        {
            geode::prelude::Mod::get()->setSavedValue<bool>(id + "_enabled", enabled);
        }

        virtual void load()
        {
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