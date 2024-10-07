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
        }

        ScrollLayer* scroll = nullptr;
        CCMenu* menu;
        std::vector<CCNode*> cells;

        static Labels* get();

        void cocosCreate(CCMenu* menu);

        void refreshList();

        void onAddItem(CCObject* sender);
        void onDelete(CCObject* sender);
        void onSettings(CCObject* sender);
};