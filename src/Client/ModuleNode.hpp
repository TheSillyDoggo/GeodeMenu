#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include <CCMenuExt.hpp>
#include <Button.hpp>

using namespace geode::prelude;

class ModuleNode : public qolmod::CCMenuExt
{
    protected:
        static inline std::vector<ModuleNode*> nodes = {};
        Module* module = nullptr;
        CCMenuItemToggler* btn = nullptr;
        qolmod::Button* infoBtn = nullptr;
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
        void onOptions(CCObject* sender);

        void onUpdateLabelColour(float dt);
        bool isDisabled();

        virtual void updateNode();
        virtual void draw();
        bool init(Module* module);
};