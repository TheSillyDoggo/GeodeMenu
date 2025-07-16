#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"

using namespace geode::prelude;

class ModuleNode : public CCMenu
{
    protected:
        static inline std::vector<ModuleNode*> nodes = {};
        Module* module = nullptr;
        CCMenuItemToggler* btn = nullptr;
        CCMenuItemToggler* favBtn = nullptr;

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

        void updateNode();
        bool init(Module* module);
};