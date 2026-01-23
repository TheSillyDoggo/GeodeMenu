#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "../Keybinds/KeyConfigStruct.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

using namespace geode::prelude;

class EditKeyConfigUI : public geode::Popup<>
{
    protected:
        BackgroundSprite* bg;
        KeyConfigStruct defaultConfig;
        KeyConfigStruct startConfig;
        KeyConfigStruct currentConfig;
        AdvLabelBMFont* infoLbl = nullptr;
        std::function<void(KeyConfigStruct)> onFinish = nullptr;

    public:
        static EditKeyConfigUI* create(std::function<void(KeyConfigStruct)> onFinish);

        void setDefaultConfig(KeyConfigStruct config);
        void setStartConfig(KeyConfigStruct config);

        void onSetDefault(CCObject* sender);
        void onUndoChanged(CCObject* sender);
        void onClose(CCObject* sender);

        virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
        virtual void update(float dt);
        virtual bool setup();
};