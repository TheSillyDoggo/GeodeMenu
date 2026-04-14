#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/ModuleShortcutConfig.hpp"
#include "BetterInputNode.hpp"
#include "PopupBase.hpp"
#include <FloatingButton/FloatingUIButton.hpp>

using namespace geode::prelude;

class SetupShortcutUI : public PopupBase
{
    protected:
        qolmod::BackgroundSprite* bg;
        std::function<void(bool, ModuleShortcutConfig)> onFinish = nullptr;
        bool shortcutEnabled = false;
        ModuleShortcutConfig conf = {};
        CCSprite* bgOffSpr = nullptr;
        CCSprite* bgOnSpr = nullptr;
        CCSprite* overlaySpr = nullptr;
        CCMenuItemToggler* enabledToggle = nullptr;
        CCMenuItemToggler* movableToggle = nullptr;
        CCMenuItemToggler* visInMenu = nullptr;
        CCMenuItemToggler* visInGame = nullptr;
        CCMenuItemToggler* visInGamePause = nullptr;
        CCMenuItemToggler* visInEditor = nullptr;
        CCMenuItemToggler* visInEditorPause = nullptr;
        BetterInputNode* scaleInput = nullptr;
        BetterInputNode* opacityInput = nullptr;
        std::unordered_map<FloatingButtonAnimationType, std::pair<CCMenuItemToggler*, AdvLabelBMFont*>> animBtns = {};
        CCSprite* colSpr = nullptr;

    public:
        std::string modID = "";

        static SetupShortcutUI* create(std::function<void(bool, ModuleShortcutConfig)> onFinish);

        void onClose(CCObject* sender);
        void onSelectSprite(CCObject* sender);
        void onSelectColour(CCObject* sender);

        void setStartConfig(bool enabled, ModuleShortcutConfig conf);
        void updateUI();
        void updateSprs();

        void addAnimButton(FloatingButtonAnimationType type, int y, CCMenu* menu);

        virtual bool setup();
        virtual void update(float dt);
};