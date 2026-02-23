#pragma once

#include "../Client/CategoryNode.hpp"
#include "IconicCommon.hpp"
#include "IconicPlayerHook.hpp"
#include <Nodes/CCNodeWithShader.hpp>

class IconEffectsUI : public CategoryNode
{
    protected:
        struct PreviewNodeData
        {
            IconicGamemodeType gamemode = IconicGamemodeType::Cube;
            bool player2 = false;
            CCMenuItemSpriteExtra* btn = nullptr;
            CCNodeWithShader* shader = nullptr;
            SimplePlayer* simple = nullptr;
            IconicPlayerHook* hook = nullptr;
        };
        struct EffectEditData
        {
            AdvLabelBMFont* label = nullptr;
            CCMenuItemToggler* toggler = nullptr;
            CCMenuItemSpriteExtra* colourBtn = nullptr;
            CCSprite* sprite = nullptr;
        };
        struct DualModeData
        {
            IconicDualMode mode;
            CCMenuItemToggler* toggler = nullptr;
            AdvLabelBMFont* label = nullptr;
        };
        std::vector<PreviewNodeData> previewNodes = {};
        std::unordered_map<IconicEffectType, EffectEditData> editNodes = {};
        std::vector<DualModeData> dualNodes = {};
        static inline std::pair<IconicGamemodeType, bool> selectedType = { IconicGamemodeType::Cube, false };
        EasyBG* dualBG = nullptr;
        EasyBG* effectsBG = nullptr;
        CCMenuItemToggler* seperateToggle = nullptr;

        void addCircleToNode(CCNode* node, float scale = 1.0f);
        void updateFineOutline();

    public:
        CREATE_FUNC(IconEffectsUI)

        std::pair<IconicGamemodeType, bool> getSelected();
        void updateSelection();
        void updateDualMode();
        void updateOverride();

        void onSelectType(CCObject* sender);
        void onToggleOverride(CCObject* sender);
        void onEditColour(CCObject* sender);
        void onToggleSeperate(CCObject* sender);
        void onDualMode(CCObject* sender);
        void onProfiles(CCObject* sender);

        void createDualNodes(IconicDualMode mode, int y, CCMenu* menu);
        void createEffectNodes(IconicEffectType type, int y, CCMenu* menu);
        CCMenuItemSpriteExtra* createPreviewButton(IconicGamemodeType gamemode, bool player2);

        virtual void update(float dt);
        bool init();
};

SUBMIT_CATEGORY("Icon Effects", IconEffectsUI)