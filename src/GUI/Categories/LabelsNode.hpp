#pragma once

#include "../../Client/CategoryNode.hpp"
#include <../UI/ScrollLayer.hpp>

class LabelsNode : public CategoryNode
{
    protected:
        static inline LabelsNode* instance = nullptr;
        CCMenu* presetMenu = nullptr;
        qolmod::ScrollLayer* labelsScroll = nullptr;
        AdvLabelBMFont* hintNone = nullptr;

        ~LabelsNode();

    public:
        CREATE_FUNC(LabelsNode)
        static LabelsNode* get();

        void onEditSafeZones(CCObject* sender);
        void onImportFromFile(CCObject* sender);
        void onAddLabel(CCObject* sender);

        void addPresets();
        void updateUI();

        // virtual void scrollWheel(float y, float x);
        bool init();
};

SUBMIT_CATEGORY("Labels", LabelsNode)