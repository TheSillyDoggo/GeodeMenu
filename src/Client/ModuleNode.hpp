#pragma once

#include <Geode/Geode.hpp>
#include "Module.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include <CCMenuExt.hpp>
#include <Button.hpp>
#include <AltMouseDelegate.hpp>

using namespace geode::prelude;

class ModuleNode : public qolmod::CCMenuExt, public qolmod::AltMouseDelegate
{
    protected:
        static inline std::vector<ModuleNode*> nodes = {};
        Module* module = nullptr;
        CCMenuItemToggler* btn = nullptr;
        qolmod::Button* infoBtn = nullptr;
        AdvLabelBMFont* label = nullptr;
        bool disabled = false;

        virtual bool altClickBegan(int button, cocos2d::CCPoint point);
        virtual void draw();

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

        virtual bool shouldAppearCheat();
        virtual bool shouldAppearDisabled();

        virtual void updateNode();
        bool init(Module* module);
};