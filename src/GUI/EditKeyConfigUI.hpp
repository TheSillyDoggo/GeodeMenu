#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "../Keybinds/KeyConfigStruct.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "PopupBase.hpp"

using namespace geode::prelude;

class EditKeyConfigUI : public PopupBase
{
    protected:
        qolmod::BackgroundSprite* bg;
        KeyConfigStruct defaultConfig;
        KeyConfigStruct startConfig;
        KeyConfigStruct currentConfig;
        AdvLabelBMFont* infoLbl = nullptr;
        std::function<void(KeyConfigStruct)> onFinish = nullptr;
        std::unordered_map<KeybindType, std::pair<CCMenuItemToggler*, AdvLabelBMFont*>> modeBtns = {};

    public:
        static EditKeyConfigUI* create(std::function<void(KeyConfigStruct)> onFinish);

        void setDefaultConfig(KeyConfigStruct config);
        void setStartConfig(KeyConfigStruct config);

        void onSetDefault(CCObject* sender);
        void onUndoChanged(CCObject* sender);
        void onSetKeyMode(CCObject* sender);
        void onModeInfo(CCObject* sender);
        void onClose(CCObject* sender);

        void createModeButton(KeybindType type, int y, CCMenu* menu);
        void updateMode(CCMenuItemToggler* except);

        virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
        virtual void update(float dt);
        virtual bool setup();
};