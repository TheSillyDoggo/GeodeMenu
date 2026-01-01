#pragma once

#include "../../Client/CategoryNode.hpp"
#include "../BetterSlider.hpp"
#include "../BetterInputNode.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

using namespace geode::prelude;

class SpeedhackNode : public CategoryNode, public TextInputDelegate
{
    protected:
        static inline SpeedhackNode* instance = nullptr;
        CCMenu* presetMenu = nullptr;
        CCMenu* presetKeybindMenu = nullptr;
        ButtonSprite* sprMode = nullptr;
        std::unordered_map<CCMenuItemSpriteExtra*, float> presetBtns = {};
        AdvLabelBMFont* deletePresetHelp = nullptr;
        CCScale9Sprite* deletePresetBG = nullptr;
        bool presetDeleteMode = false;

        CCMenuItemSpriteExtra* trashBtn = nullptr;
        BetterSlider* slider = nullptr;
        BetterInputNode* input = nullptr;

        ~SpeedhackNode();

    public:
        CREATE_FUNC(SpeedhackNode)
        static SpeedhackNode* get();

        void onToggleEnabled(CCObject* sender);
        void onToggleMusic(CCObject* sender);
        void onToggleGameplay(CCObject* sender);
        void onTrash(CCObject* sender);
        void onSliderMoved(CCObject* sender);

        void onApplyPreset(CCObject* sender);
        void onRemovePreset(CCObject* sender);
        void onAddNewPreset(CCObject* sender);
        void onChangePresetMode(CCObject* sender);
        void onSetPresetKeybind(CCObject* sender);

        void updatePresets();

        void updateUI();
        virtual void textChanged(CCTextInputNode* node);

        bool init();
};

SUBMIT_CATEGORY("Speedhack", SpeedhackNode)