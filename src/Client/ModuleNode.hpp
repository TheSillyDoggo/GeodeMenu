#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

using namespace geode::prelude;

class ModuleNode : public CCMenu
{
    protected:
        static inline std::vector<ModuleNode*> nodes = {};
        Module* module = nullptr;
        CCMenuItemToggler* btn = nullptr;
        CCMenuItemToggler* favBtn = nullptr;
        CCMenuItemSpriteExtra* infoBtn = nullptr;
        AdvLabelBMFont* label = nullptr;
        bool disabled = false;

        ~ModuleNode();
    
    public:
        static ModuleNode* create(Module* module);
        static void updateAllNodes(ModuleNode* except);

        virtual void setup();

        void onToggle(CCObject* sender);
        void onToggleError(CCObject* sender);
        void onInfo(CCObject* sender);
        void onToggleFavourite(CCObject* sender);
        void onInfoToggleFavourite(CCObject* sender);
        void onOptions(CCObject* sender);
        void onChangeKeybind(CCObject* sender);
        void onChangeShortcut(CCObject* sender);

        void onUpdateLabelColour(float dt);
        bool isDisabled();

        virtual void updateNode();
        virtual void draw();
        bool init(Module* module);
};