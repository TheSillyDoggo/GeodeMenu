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

        static Labels* get();

        void cocosCreate(CCMenu* menu);

        void refreshList();

        void save();
        void load();
        void loadFromPrevSave();

        void onAddItem(CCObject* sender);
        void onDelete(CCObject* sender);
        void onSettings(CCObject* sender);
        void onSetupSafeZone(CCObject* sender);
};