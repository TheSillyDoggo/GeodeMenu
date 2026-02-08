#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "../Utils/ColourConfig.hpp"
#include "BetterSlider.hpp"
#include "PopupBase.hpp"
#include "BetterInputNode.hpp"

using namespace geode::prelude;

class SetupColourConfigUI : public PopupBase, public ColorPickerDelegate, public TextInputDelegate
{
    protected:
        bool allowEffects = false;
        BackgroundSprite* bg = nullptr;
        ColourConfig defaultConfig;
        ColourConfig startConfig;
        ColourConfig currentConfig;
        std::function<void(ColourConfig)> onFinishFunc = nullptr;
        CCLayerColor* startColour = nullptr;
        CCLayerColor* endColour = nullptr;
        std::string previewChannel = "";
        CCControlColourPicker* picker = nullptr;
        CCMenu* typeMenu = nullptr;
        CCMenu* bottomLeft = nullptr;
        std::map<CCMenuItemToggler*, ColourConfigType> configTypes = {};
        CCNode* gradientPreviewContainer = nullptr;
        std::unordered_map<CCLayerColor*, float> gradientPreviewSprites = {};
        CCNode* gradientTimePreview = nullptr;
        std::vector<CCNode*> gradientLines = {};
        int selectedGradientLine = 0;
        CCMenu* gradientLineConfigNode = nullptr;
        CCSprite* gradientLineColour = nullptr;
        BetterSlider* gradientLineLocation = nullptr;
        CCMenuItemSpriteExtra* gradientAddStepBtn = nullptr;
        CCMenuItemSpriteExtra* gradientDelStepBtn = nullptr;
        BetterSlider* speedSlider = nullptr;
        BetterInputNode* speedInput = nullptr;
        CCNode* colArea = nullptr;
        BetterInputNode* rInput = nullptr;
        BetterInputNode* gInput = nullptr;
        BetterInputNode* bInput = nullptr;
        BetterInputNode* hexInput = nullptr;

        virtual void textChanged(CCTextInputNode* node);

    public:
        static SetupColourConfigUI* create(std::function<void(ColourConfig)> onFinishFunc, bool allowEffects = true);

        void onClose(CCObject* sender);
        void onChangeType(CCObject* sender);
        void onSetDefault(CCObject* sender);
        void onUndoChanged(CCObject* sender);
        void onSelectGradientLine(CCObject* sender);
        void onChangeGradientLineColour(CCObject* sender);
        void onGradientLocationSlider(CCObject* sender);
        void onAddGradientStep(CCObject* sender);
        void onDeleteGradientStep(CCObject* sender);
        void onSpeedSliderChanged(CCObject* sender);

        void setStartConfig(ColourConfig config);
        void setDefaultConfig(ColourConfig config);
        void updateUI();
        void updateInputs(BetterInputNode* except);

        void setPreviewChannel(std::string channel);

        void addTypeButtons(CCMenu* menu);
        void updateTypeButtons(CCMenuItemToggler* excluding);

        void createGradientPreview();
        void updateGradientPreview();
        CCNode* createGradientLine(bool timePreview, ccColor3B col = ccWHITE, bool selected = false, int tag = 0);
        void updateGradientLines();

        virtual void colorValueChanged(ccColor3B colour);
        virtual void update(float dt);
        virtual bool setup();
};