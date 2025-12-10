#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "../Utils/ColourConfig.hpp"

using namespace geode::prelude;

class SetupColourConfigUI : public geode::Popup<>, public ColorPickerDelegate
{
    protected:
        BackgroundSprite* bg = nullptr;
        ColourConfig defaultConfig;
        ColourConfig startConfig;
        ColourConfig currentConfig;
        std::function<void(ColourConfig)> onFinishFunc = nullptr;
        CCLayerColor* startColour = nullptr;
        CCLayerColor* endColour = nullptr;
        std::string previewChannel = "";
        CCControlColourPicker* picker = nullptr;
        std::map<CCMenuItemToggler*, ColourConfigType> configTypes = {};

    public:
        static SetupColourConfigUI* create(std::function<void(ColourConfig)> onFinishFunc);

        void onClose(CCObject* sender);
        void onChangeType(CCObject* sender);
        void onSetDefault(CCObject* sender);

        void setStartConfig(ColourConfig config);
        void setDefaultConfig(ColourConfig config);
        void updateUI();

        void setPreviewChannel(std::string channel);

        void addTypeButtons(CCMenu* menu);
        void updateTypeButtons(CCMenuItemToggler* excluding);

        virtual void colorValueChanged(ccColor3B colour);
        virtual void update(float dt);
        virtual bool setup();
};