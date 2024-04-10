#pragma once

#include "../Client/Client.h"
#include "../Labels/Labels.h"

class Labels : public Window
{
    public:
        Labels()
        {
            name = "Labels";
            id = "labels-window";
        }

        std::vector<CCLabelBMFont*> labels = {};

        void cocosCreate(CCMenu* menu);

        void onHide(CCObject*);

        void onToggleLabel(CCObject* sender);

        void onLabelOptions(CCObject* sender);
};