#pragma once

#include "../../Client/CategoryNode.hpp"
#include "../BetterSlider.hpp"

using namespace geode::prelude;

class SpeedhackNode : public CategoryNode, public TextInputDelegate
{
    protected:
        CCMenu* presetMenu = nullptr;
        ButtonSprite* sprMode = nullptr;
        std::unordered_map<CCMenuItemSpriteExtra*, float> presetBtns = {};
        CCLabelBMFont* deletePresetHelp = nullptr;
        CCScale9Sprite* deletePresetBG = nullptr;
        bool presetDeleteMode = false;

        CCMenuItemToggler* enabledBtn = nullptr;
        CCMenuItemToggler* musicBtn = nullptr;
        CCMenuItemToggler* gameplayBtn = nullptr;
        CCMenuItemSpriteExtra* trashBtn = nullptr;
        BetterSlider* slider = nullptr;
        TextInput* input = nullptr;

    public:
        CREATE_FUNC(SpeedhackNode)

        void onToggleEnabled(CCObject* sender);
        void onToggleMusic(CCObject* sender);
        void onToggleGameplay(CCObject* sender);
        void onTrash(CCObject* sender);
        void onSliderMoved(CCObject* sender);

        void onApplyPreset(CCObject* sender);
        void onRemovePreset(CCObject* sender);
        void onAddNewPreset(CCObject* sender);
        void onChangePresetMode(CCObject* sender);

        void updatePresets();

        virtual void textChanged(CCTextInputNode* node);

        bool init();
};

SUBMIT_CATEGORY("Speedhack", SpeedhackNode)