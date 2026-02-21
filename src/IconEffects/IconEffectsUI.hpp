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
        std::vector<PreviewNodeData> previewNodes = {};
        std::pair<IconicGamemodeType, bool> selectedType = { IconicGamemodeType::Cube, false };

    public:
        CREATE_FUNC(IconEffectsUI)

        void updateSelection();

        void onSelectType(CCObject* sender);
        void onTest(CCObject* sender);
        void onTest2(CCObject* sender);
        void onTest3(CCObject* sender);

        CCMenuItemSpriteExtra* createPreviewButton(IconicGamemodeType gamemode, bool player2);

        virtual void update(float dt);
        bool init();
};

SUBMIT_CATEGORY("Icon Effects", IconEffectsUI)