#pragma once

#include "../Client.h"
#include "../../Labels/Labels.h"

class Labels : public Window
{
    private:
        static inline Labels* instance = nullptr;
    public:
        Labels()
        {
            name = "Labels";
            id = "labels-window";

            instance = this;

            load();
        }

        ScrollLayer* scroll = nullptr;
        CCMenu* menu;
        std::vector<CCNode*> cells;
        CCRect safeZone;
        std::function<CCLabelBMFont*()> createFuncCommentEmojis = nullptr;

        static Labels* get();

        void cocosCreate(CCMenu* menu);

        void refreshList();
        void dirtyRefreshList(bool smooth);

        void save();
        void load();
        void loadFromPrevSave();

        void onAddItem(CCObject* sender);
        void onDelete(CCObject* sender);
        void onSettings(CCObject* sender);
        void onSetupSafeZone(CCObject* sender);
        void onMoveLabelUp(CCObject* sender);
        void onMoveLabelDown(CCObject* sender);
        void onToggleVisible(CCObject* sender);
        void onImportFromFile(CCObject* sender);

        void importFromFile(std::filesystem::path path);
};